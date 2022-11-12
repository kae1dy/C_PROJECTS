int satsum(int v1, int v2) { 
    enum { MY_INT_MAX = (unsigned int) -1 >> 1};
    enum { MY_INT_MIN = ~MY_INT_MAX};

    if (v1 < 0 && v2 < 0) {
        int temp = MY_INT_MIN / 2;
        if ((v1 < temp && v2 <= temp) || (v2 < temp && v1 <= temp)) {
            return MY_INT_MIN;
        }
        if ((v1 < temp && v2 + (v1 - temp) < temp) || (v2 < temp && v1 + (v2 - temp) < temp)) {
            return MY_INT_MIN;
        }
    }

    else if (v1 > 0 && v2 > 0) {
        int temp = MY_INT_MAX / 2;
        if (v1 > temp && v2 > temp) {
            return MY_INT_MAX;
        }
        if ((v1 > temp && v2 + (v1 - temp) > temp + 1) || (v2 > temp && v1 + (v2 - temp) > temp + 1)) {
            return MY_INT_MAX;
        }
    }
    return v1 + v2;
}
