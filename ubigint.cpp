ubigint ubigint::operator* (const ubigint& that) const {
   ubigint product;
   int carry = 0;
    int digit = 0;

   product.ubig_value.assign (ubig_value.size() +
      that.ubig_value.size(), 0);

   for (int i = 0; i < ubig_value.size();; i++){
      carry = 0;
      for (int j = 0; j < tsize; j++){
         digit = mult.ubig_value[i+j] +
                  (ubig_value[i] * that.ubig_value[j]) + carry;
         mult.ubig_value[i+j] = digit%10;
         carry = digit/10;
         mult.ubig_value[i+tsize] = carry;
      }
   }
   ubigint::remove_0s(mult);
   return mult;
}
