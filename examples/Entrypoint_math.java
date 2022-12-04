class Entrypoint {

    public static void println(String s) {}
    public static void println(int s) {}
    public static void println(float s) {}
    public static void beep(float frequency, int duration) {}
    public static void delay(int duration_ms) {}
    
    public static void main(String argv[]) {
        int a = 4232;
        int b = 123;
        float c = a / b;
        c = c + 345 - 1231 * b / a;

        

        Entrypoint.println("Hello, math world!");
        Entrypoint.delay(3000);
        Entrypoint.println(c);
        Entrypoint.delay(500);
        a = -5 - 10 * 1231 / 1231;
        Entrypoint.println(a);
        Entrypoint.delay(5000);
    }
}