"""
#    Password Cracker
#    
#    generates all possible passwords candidates
#    given alphabet set and minimum/maximum password length
#    (generated in provided alphabetical order)
#
#    author: rbcorx
"""

alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" # alphabet set to generate password from
def get_password_candidates(pass_min_length=1, pass_max_len=5):	
	password_l = []
	password = [0, ] +  [-1, ] * (pass_max_len - 1)
	iteration_count = 0
	for pass_len in range(pass_min_length, pass_max_len+1):
		lev = 2
		password = [-1 for _ in password]
		password[0] = 0
		while lev:
			if lev-1 >= pass_len:
				iteration_count += 1
				if iteration_count % 1000000 == 0:
					print ("1M candidates generated!", password_l[-1])
				password_l.append("".join(map(lambda i: alpha[i], password[:pass_len])))
				lev = pass_len
			password[lev-1] += 1
			if password[lev-1] >= len(alpha):
				password[lev-1] = -1
				lev -= 1
				continue
			lev += 1
	return password_l

def test():
	mi_len = 1
	mx_len = 5
	password_l = get_password_candidates(mi_len, mx_len)
	expected_count = sum([len(alpha)**x for x in range(mi_len, mx_len+1)])
	count_test = len(password_l) == expected_count
	alphabetical_order_test = reduce(lambda x, y: x < y, password_l)
	test_report = lambda test_name, test_result: "{} test {}!".format(test_name, "passed" if test_result else "failed")
	print(test_report("expected count", count_test))
	print(test_report("alphabetical order", alphabetical_order_test))
	

def main():
	get_password_candidates()

if __name__ == "__main__":
	main()
