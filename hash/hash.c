int m = 30;
int h = 0;
int hash_wein(char * p) {
    int g;
    while (*p != '\0') {
        h = (h << 4) + (*p);
        g = h;
        if (g & 0xf0000000)   {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
        p++;
    }
    return h % m;
}