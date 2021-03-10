
def raw_to_nano(raw_amount):
	raw = "1000000000000000000000000000000"
	diff = len(raw)-len(raw_amount)

	if(diff>0):
		# smaller than 1 nano

		nano_amount = "0." + (diff-1) * "0" + raw_amount[0:-1]
	elif (diff==0):
		# between 1 and 9 nano
		nano_amount = raw_amount[0] + "." + raw_amount[1:-1]
	else:
		# more than 9 nano
		nano_amount = raw_amount[0:-diff+1] + "." + raw_amount[-diff+1:-1]

	return(nano_amount.rstrip("0"))




test_amounts = {
	"12345000000000000000000000000000" : "12.345",
	"350000000000000000000000000000" : "0.35",
	"98000000000000000000000000000" : "0.098",
	"1980000000000000000000000" : "0.0000019800",
	"347983400000000" : "0.0000000000000003479834",
	"1200000000000000000000000000000" : "1.2"
}


for raw_amount in test_amounts:
	nano = raw_to_nano(raw_amount)
	print(raw_amount, test_amounts[raw_amount], len(test_amounts[raw_amount]), nano, len(nano))

