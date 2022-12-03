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
        Entrypoint.println("Hello, world!");
        Entrypoint.delay(3000);
        Entrypoint.println("This is a working message!");
        Entrypoint.delay(3000);
        Entrypoint.println("Never gonna give you up");
        Entrypoint.beep(440, 1000);
        Entrypoint.delay(1000);
        Entrypoint.println("Never gonna let you down");
        Entrypoint.beep(250, 1000);
        Entrypoint.delay(1000);
        Entrypoint.println("Never gonna run around and desert you");
        Entrypoint.delay(1000);
        Entrypoint.println("Never gonna make you cry");
        Entrypoint.delay(1000);
        Entrypoint.println("Never gonna say goodbye");
        Entrypoint.delay(1000);
        Entrypoint.println("Never gonna tell a lie and hurt you");
        Entrypoint.delay(5000);
    }
}