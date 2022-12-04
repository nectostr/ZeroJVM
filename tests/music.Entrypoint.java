class Entrypoint {
    float field1 = 4.12431134f;
    Entrypoint() {}

    public static void println(String s) {}
    public static void println(int s) {}
    public static void println(float s) {}
    public static void beep(float frequency, int duration) {}
    public static void delay(int duration_ms) {}

    public static void main(String argv[]) {
//         Entrypoint.println("Hello, Java World! ^_^");
        Entrypoint var = new Entrypoint();
        Entrypoint.println(var.field1);
//         Entrypoint.beep(440, 1000);
//         Entrypoint.delay(10000);
    }
}