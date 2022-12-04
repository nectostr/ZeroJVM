class Entrypoint {
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
        this.field2 = 5;
    }
    Entrypoint(int a) {
        this.field3 = a;
    }

    public static void println(String s) {}
    public static void beep(float frequency, int duration) {}
    public static void delay(int duration_ms) {}

    public static void main(String argv[]) {
        Entrypoint.println("Hello, Java World! ^_^");
        Entrypoint.delay(10000);
    }
}