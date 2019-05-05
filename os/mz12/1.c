STYPE bit_reverse(STYPE value)
{
    UTYPE a = (UTYPE) value;
    
    UTYPE test = 0;
    test = ~test;
    int count = 0;
    while(test > 0) {
        count++;
        test >>= 1;
    }
    
    UTYPE b = 0;
    for(int i = 0; i < count; i++){
        b |= (a & 1);
        if(i != count - 1) {
            b <<= 1;
        }
        a >>= 1;
    }
    return (STYPE)b;
}
