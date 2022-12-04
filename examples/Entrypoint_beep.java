class Entrypoint {
    public static void println(String s) {}
    public static void beep(float frequency, int duration) {}
    public static void delay(int duration_ms) {}
/*
2G4, 2F#4, 2E4, 2E4, 2F#4, P...., 
2A3, 2G4, 2F#4, 2E4, E4, 2F#4, P,
 D4, 2E4, A3, P...,
  2A3, E4, 2F#4, G4.., 
  2E4, C#4, D4, E4, 2A3, A3, F#4, P...., 
  2G4, 2F#4, 2E4, 2E4, 2F#4, P...., 
  2A3, 2G4, 2F#4, 2E4, E4, 2P, 
  2F#4, D4, 2P, 2E4, A3, P..., 
  E4, 2F#4, G4., 2E4, C#4., 
  2D4, E4, 4P, 2A3, 2D4, 2E4,
   2F4, 2E4, 2D4, 2C4, P, 2A3, 
   2A#3, C4, F4, 2E4, 2D4, 2D4,
    2C4, 2D4, 2C4, C4, C4, 2A3, 
    2A#3, C4, F4, 2G4, 2F4, 2E4, 
    2D4, 2D4, 2E4, F4, F4, 2G4, 2A4,
     2A#4, 2A#4, A4, G4, 2F4, 2G4, 2A4,
      2A4, G4, F4, 2D4, 2C4, 2D4, 2F4,
       2F4, E4, 2E4, 2F#4, F#4..., P..
*/
    public static void main(String argv[]) {
        Entrypoint.println("The cake is not a lie! ^_^");
        Entrypoint.delay(1000);
        // 4th octave
        // Entrypoint.beep(392, 250);
        // Entrypoint.beep(370, 250);
        // Entrypoint.beep(329.6f, 250);
        // Entrypoint.beep(329.6f, 250);
        // Entrypoint.beep(370, 250);
        // Entrypoint.delay(250);
        // 5th octave
        // G5
        Entrypoint.beep(783.99f, 250);
        // F#5
        Entrypoint.beep(739.99f, 250); 
        // E5
        Entrypoint.beep(659.25f, 250);
        Entrypoint.delay(10);
        // E5
        Entrypoint.beep(659.25f, 250); 
        // F#5
        Entrypoint.beep(739.99f, 250);
        Entrypoint.delay(250*10);
        // A4
        Entrypoint.beep(440.0f, 250); 
        // G5 
        Entrypoint.beep(783.99f, 250); 
        // F#5
        Entrypoint.beep(739.99f, 250);  
        // E5 
        Entrypoint.beep(659.25f, 250); 
        Entrypoint.delay(10);
        // E5
        Entrypoint.beep(659.25f, 3*250);   
        // F#5  
        Entrypoint.beep(739.99f, 250); 
        // D5
        Entrypoint.beep(587.33f, 3*250); 
        // E5
        Entrypoint.beep(659.25f, 250);  
        // A4
        Entrypoint.beep(440.0f, 4*250); 
        Entrypoint.delay(250*5);

        // A4
        Entrypoint.beep(440.0f, 250);
        // E5
        Entrypoint.beep(659.25f, 2*250);
        // F#5
        Entrypoint.beep(739.99f, 250);
        // G5
        Entrypoint.beep(783.99f, 3*250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // C#5
        Entrypoint.beep(554.37f, 3*250);
        // D5
        Entrypoint.beep(587.33f, 250);
        // E5
        Entrypoint.beep(659.25f, 3*250);
        // A4
        Entrypoint.beep(440.0f, 250);
        Entrypoint.delay(10);
        // A4
        Entrypoint.beep(440.0f, 2*250);
        // F#5
        Entrypoint.beep(739.99f, 3*250);

        Entrypoint.delay(250*8);

        // 2nd quadrant
        // G5
        Entrypoint.beep(783.99f, 250);
        // F#5
        Entrypoint.beep(739.99f, 250); 
        // E5
        Entrypoint.beep(659.25f, 250);
        Entrypoint.delay(10);
        // E5
        Entrypoint.beep(659.25f, 250); 
        // F#5
        Entrypoint.beep(739.99f, 250);
        Entrypoint.delay(250*10);
        // A4
        Entrypoint.beep(440.0f, 250); 
        // G5 
        Entrypoint.beep(783.99f, 250); 
        // F#5
        Entrypoint.beep(739.99f, 250);  
        // E5 
        Entrypoint.beep(659.25f, 250); 
        Entrypoint.delay(10);
        // E5
        Entrypoint.beep(659.25f, 3*250);   
        // F#5  
        Entrypoint.beep(739.99f, 250); 
        // D5
        Entrypoint.beep(587.33f, 3*250); 
        // E5
        Entrypoint.beep(659.25f, 250);  
        // A4
        Entrypoint.beep(440.0f, 4*250); 
        Entrypoint.delay(250*4);

        // A4
        Entrypoint.beep(440.0f, 250);
        // E5
        Entrypoint.beep(659.25f, 2*250);
        // F#5
        Entrypoint.beep(739.99f, 250);
        // G5
        Entrypoint.beep(783.99f, 3*250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // C#5
        Entrypoint.beep(554.37f, 3*250);
        // D5
        Entrypoint.beep(587.33f, 250);
        // E5
        Entrypoint.beep(659.25f, 2*250);
        // A4
        Entrypoint.beep(440.0f, 250);
        // D5
        Entrypoint.beep(587.33f, 250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // F5
        Entrypoint.beep(698.46f, 250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // D5
        Entrypoint.beep(587.33f, 250);
        // C5
        Entrypoint.beep(523.25f, 250);
        Entrypoint.delay(2*250);
        // correct

        // chorus
        // A4
        Entrypoint.beep(440.0f, 250);
        // A#4
        Entrypoint.beep(466.16f, 250);
        // C5
        Entrypoint.beep(523.25f, 2*250);
        // F5
        Entrypoint.beep(698.46f, 2*250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // D5
        Entrypoint.beep(587.33f, 250);
        Entrypoint.delay(10);
        // D5
        Entrypoint.beep(587.33f, 250);
        // C5
        Entrypoint.beep(523.25f, 250);
        // D5
        Entrypoint.beep(587.33f, 250);
        // C5
        Entrypoint.beep(523.25f, 250);
        Entrypoint.delay(10);
        // C5
        Entrypoint.beep(523.25f, 2*250);
        Entrypoint.delay(10);
        // C5
        Entrypoint.beep(523.25f, 2*250);

        // A4
        Entrypoint.beep(440.0f, 250);
        // A#4
        Entrypoint.beep(466.16f, 250);
        // C5
        Entrypoint.beep(523.25f, 2*250);
        // F5
        Entrypoint.beep(698.46f, 2*250);
        // G5
        Entrypoint.beep(783.99f, 250);
        // F5
        Entrypoint.beep(698.46f, 250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // D5
        Entrypoint.beep(587.33f, 250);
        Entrypoint.delay(10);
        // D5
        Entrypoint.beep(587.33f, 250);
        // E5
        Entrypoint.beep(659.25f, 250);
        // F5
        Entrypoint.beep(698.46f, 2*250);
        Entrypoint.delay(10);
        // F5
        Entrypoint.beep(698.46f, 2*250);
        // G5
        Entrypoint.beep(783.99f, 250);
        // A5
        Entrypoint.beep(880.0f, 250);
        // A#5
        Entrypoint.beep(932.33f, 250);
        Entrypoint.delay(10);
        // A#5
        Entrypoint.beep(932.33f, 250);
        // A5
        Entrypoint.beep(880.0f, 2*250);
        // G5
        Entrypoint.beep(783.99f, 2*250);
        // F5
        Entrypoint.beep(698.46f, 250);
        // G5
        Entrypoint.beep(783.99f, 250);
        // A5
        Entrypoint.beep(880.0f, 250);
        Entrypoint.delay(10);
        // A5
        Entrypoint.beep(880.0f, 250);
        // G5
        Entrypoint.beep(783.99f, 2*250);
        // F5
        Entrypoint.beep(698.46f, 2*250);

        // D5
        Entrypoint.beep(587.33f, 250);
        // C5
        Entrypoint.beep(523.25f, 250);
        // D5
        Entrypoint.beep(587.33f, 250);
        // F5
        Entrypoint.beep(698.46f, 250);
        Entrypoint.delay(10);
        // F5
        Entrypoint.beep(698.46f, 250);
        // E5
        Entrypoint.beep(659.25f, 2*250);
        Entrypoint.delay(10);
        // E5
        Entrypoint.beep(659.25f, 250);
        // F#5
        Entrypoint.beep(739.99f, 250);
        Entrypoint.delay(10);
        // F#5
        Entrypoint.beep(739.99f, 3*250);
    }
}