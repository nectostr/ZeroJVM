class Simple {
    static int field1 = 4;
    static int fielda = 5;
    int field2 = 3;
    int field3 = 7;
    static int field4;
    static int m1(int a) {
        return a + 9;
    }
    int m2(int a, int b, int c) {
        return a + b * c;
    }
    void m3() {

    }
    Simple() {

    }
    public static void main(String argv[]) {
        int i = m1(78);
        Simple s = new Simple();
        int b = s.m2(s.field1, 1324, i);
        if (b > 0) {
            s.m3();
        }
    }
}