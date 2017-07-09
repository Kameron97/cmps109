void ubigint::divide_by_2() {
	int remainder = 0;
	for (int i = ubig_value.size()-1; i >= 0; i--) {
		int digitQuotient = 0;
		if (remainder > 0) {
			digitQuotient += remainder;
			remainder = 0;
		}

		digitQuotient += ubig_value.at(i) / 2;

		if (ubig_value.at(i) % 2) {
			remainder = 5;
		}

		ubig_value.at(i) = digitQuotient;
	}

	while (ubig_value.size() > 0 and ubig_value.back() == 0) ubig_value.pop_back();
}
