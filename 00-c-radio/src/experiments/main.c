#include <habcore/habcore.h>
#include <habcore/uart.h>
#include <habcore/rtx.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

extern const uint8_t PROGMEM img_data[];
extern const uint16_t img_len;

// Run once before the main loop
void setup(void) {
    uart_start(9600);
    rtx_start(50);
    delay(5000);
}

// send the image data encoded in data.c
void send_img(void) {
    for(uint16_t i = 0; i < img_len; ++i) {
        rtx_write(pgm_read_byte_near(img_data + i));
    }
}

// main loop
void loop(void) {
    static uint16_t packetID = 0;
    static char telemetryString[64];
    
    // Create a telemetry beacon string and transmit it
    snprintf(telemetryString, 64, "$$ABTY:BCN:%04d\n", packetID++);
    rtx_write_bytes(telemetryString, 16);
    
    // Send the image after waiting 10 seconds
    delay(10000);
    send_img();
}
