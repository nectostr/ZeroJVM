class Entrypoint {
    static int field1 = 4;
    int field2 = 3;
    static int meth1(int a) {
        return a + self.field1;
    }
    int meth2(int a, int b, int c) {
        return Entrypoint.field2 + a + b * c;
    }
    

    public static void println(String s) {}
    public static void beep(float frequency, int duration) {}
    public static void delay(int duration_ms) {}

    public static void main(String argv[]) {
        Entrypoint.println("Hello, world!");
        Entrypoint.delay(3000);
        Entrypoint e1 = new Entrypoint();
        Entrypoint e2 = new Entrypoint();
        e1.field2 = 5;
        Entrypoint.println("", Entrypoint.meth1(5), e1.meth2(1, 2, 3), e2.meth2(1, 2, 3));
        Entrypoint.delay(3000);
    }
}