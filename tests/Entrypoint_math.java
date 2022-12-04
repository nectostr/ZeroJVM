class Entrypoint {

    public static void println(String s) {}
    public static void println(int s) {}
    public static void println(float s) {}
    public static void beep(float frequency, int duration) {}
    public static void delay(int duration_ms) {}
    
    public static void main(String argv[]) {
        int a = 4232;
        int b = 123;
        int c = a / b + 345 - 1231 * a;

        

        Entrypoint.println("Hello, world!");
        Entrypoint.delay(3000);
        Entrypoint.println(c);
        Entrypoint.delay(500);
        Entrypoint.println(2567*0 + 0 - 9);
        Entrypoint.delay(5000);
    }
}