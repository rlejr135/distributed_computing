struct two_in{
	int a;
	int b;
};

program CAL{
	version ASSI_1{
		int ADDITION(struct two_in) = 1;
		int SUBTRACTION(struct two_in) = 2;
		int MULTIPLICATION(struct two_in) = 3;
		int DIVISION(struct two_in) = 4;
	} = 1;
} = 0x20fff100;
