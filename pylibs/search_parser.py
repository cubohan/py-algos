"""
flags
filters:
    string_match partial/exact
    where clauses exact
        flags
# TODO lt/gt/eq comparisions; range queries
Search field priority/ranking
# TODO remove unescape //check
query_element:
<field1,field2,field3, ...; optional> <operator; optional> <value-to-search; required>
search_query:
query_element <boolean_relationship> query_element ...
a field may have abbreviations/aliases which may have mappings to multiple fields
# add search validation for fields
"""


def partial_str_comp():
    return "{} LIKE '%{}%'"


def exact_comp():
    return "{} = '{}'"


def starts_with_comp():
    return "{} LIKE '{}%'"


def ends_with_comp():
    return "{} LIKE '%{}'"


comp_skeleton_mapping = {
    ":": exact_comp(),
    ":=": exact_comp(),
    ":~": partial_str_comp(),
    ":=~": starts_with_comp(),
    ":~=": ends_with_comp(),
}


def is_escaped(string, i):
    return i > 0 and string[i - 1] == '\\'


def fragment_filter_query(filter_query):
    """
    different filters should be separated with ' & ' or ' | '
    query string may be enclosed within quotes "<query-string>"
    inner quotes may be escaped with a '\'
    deprecated.
        inner spaces may be escaped with a '\'
    """
    filter_query = filter_query.strip()
    str_delimiters = ["\"", ]
    filter_rels_delimiters = ['&', '|', ]
    inbtw = False
    filters = []
    filters_rel = []
    ls = 0

    for i, c in enumerate(filter_query):
        if is_escaped(filter_query, i):
            continue
        if c in str_delimiters:
            inbtw = not inbtw
        elif c in filter_rels_delimiters:
            if (not inbtw):
                filters.append(filter_query[ls:i])
                filters_rel.append(c)
                ls = i + 1

    filters.append(filter_query[ls:])

    # removing edge relationship characters
    for i in [0, -1]:
        if filters[i] in filter_rels_delimiters:
            filters.pop(i)

    return (filters, filters_rel)


def field_parser(field):
    fields = field.split(",")
    return fields


def filter_parser(filter_field_q):
    comp_delimiters = [':', ]
    comp_scopes = ["=", "~", ]
    str_delimiters = ["\"", ]
    comp_index = 0
    comp = ""
    field = ""
    val = ""
    inbtw = False
    for i, c in enumerate(filter_field_q):
        if is_escaped(filter_field_q, i):
            continue
        if c in str_delimiters:
            inbtw = not inbtw
        if c in comp_delimiters and (not inbtw):
            comp_index = i
            i += 1
            while i < len(filter_field_q) and (filter_field_q[i] in comp_scopes):
                i += 1
                continue
            if (filter_field_q[i] in comp_scopes):
                print("Empty value! Invalid query")
                return None
            field = filter_field_q[0:comp_index]
            comp = filter_field_q[comp_index:i]
            val = filter_field_q[i:]

    if not comp:
        # field and comp are None
        val = filter_field_q
    # unescaping escaped characters
    for term in (field, val):
        i = 1
        while i < len(term):
            if is_escaped(term, i):
                term = term[0:i - 1] + term[i:]
                continue
            i += 1

    # cleaning extracted values
    field = field.strip()
    val = val.strip()
    if val.startswith("\""):
        val = val[1:]
    if val.endswith("\""):
        val = val[:-1]

    return (field_parser(field), val, comp)


def get_field(field):
    fmap = {
        "locale": ["account.BillingStreet",
                   "account.BillingCity",
                   "account.BillingState",
                   "account.BillingCountry"],
        "user": ["Account_Manager_Email__c", "Owner.Username"],
    }
    # TODO provide field abbreviations
    # TODO MAP field abstraction to multiple fields in SFDC
    if field in fmap:
        return fmap[field]

    return field


def filter_builder(field_filter):
    """
    ARGS:
        field_query: tuple (field_name, value, comparision_type)
    # Resolved. TODO CASE: field/comp are NONE
    # TODO CASE: val is 16 digit ID??
    """
    default_fields = ["Id", "Name"]
    default_comp = [":", ":~"]

    def _get_default_fields(val):
        if val.__len__() == 15 or val.__len__() == 18:
            return default_fields
        return default_fields[1:]

    def _get_default_comp(fields):
        if len(fields) > 1:
            return default_comp
        return default_comp[1:]

    def _build(field, val, comp):
        if comp not in comp_skeleton_mapping:
            # invalid query
            print("invalid query: unrecognized comparator")
            return None
        return comp_skeleton_mapping[comp].format(field, val)

    fields, val, comp = field_filter

    if len(fields) == 1 and (not fields[0]):
        fields = _get_default_fields(val)
    if not comp:
        comp = _get_default_comp(fields)

    res = []
    for i, field in enumerate(fields):

        # Monkey Patch. For case when comp itself is a list.
        # Evades Id error in default cases where comp choices are limited
        cur_comp = comp
        if type(comp) is not str:
            cur_comp = comp[i]

        # Monkey Patch. mutating fields from shortform
        field = get_field(field)
        if type(field) is not str:
            for f in field:
                res.append(_build(f, val, cur_comp))
            continue

        res.append(_build(field, val, cur_comp))

    return res


def combine_filters(filters, filters_rel, delimit=False):
    rel_mapping = {
        "&": "AND",
        "|": "OR",
    }

    flat = []
    for i, rel in enumerate(filters_rel):
        flat.append(filters[i])
        if rel not in rel_mapping:
            print("invalid relationship between filters, invalid query!")
            break
        flat.append(rel_mapping[rel])
    flat.append(filters[-1])

    res = " ".join(flat)
    return "({})".format(res) if delimit else res


def process_filters(filters, filters_rel):
    # TODO unescape escaped characters
    default_rel = "|"

    filters = map(filter_builder, map(filter_parser, filters))

    filters_flat = []

    for i, filter_grp in enumerate(filters):

        combi = filter_grp[0]
        if filter_grp.__len__() > 1:
            combi = combine_filters(filter_grp,
                                    [default_rel] * (len(filter_grp) - 1),
                                    delimit=True)
        filters_flat.append(combi)

    return combine_filters(filters_flat, filters_rel)


def test_filters(test_q):
    """
    test query examples:
    "in,xd,nana:lol & Id:110 | back,field2,field3:~lmao"
    "stephanie.tapprich@sojern.com"
    ":stephanie.tapprich@sojern.com"
    "Owner.Username,Account_Manager_Email__c:stephanie.tapprich@sojern.com"
    """
    return process_filters(*fragment_filter_query(test_q))


if __name__ == "__main__":
    testq = "in,xd,nana:lol & Id:110 | back,field2,field3:~lmao"
    while True:
        if not testq:
            testq = input("Enter query...\n")
        print(test_filters(testq))
        testq = None
