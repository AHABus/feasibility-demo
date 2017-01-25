
#include <habcore/rtx.h>
#include <habcore/utils.h>
#include <avr/interrupt.h>

#define RTX_BUFFER_SIZE 256
#define RTX_PIN 0

typedef enum rtx_state_e {
    STATUS_IDLE,
    STATUS_START,
    STATUS_DATABIT,
    STATUS_STOP,
} rtx_state_t;

static volatile uint8_t _rtx_tx[RTX_BUFFER_SIZE];
static volatile index_t _rtx_head;
static volatile index_t _rtx_tail;

static volatile byte_t _rtx_byte;
static volatile uint8_t _rtx_counter;
static volatile rtx_state_t _rtx_state;


ISR(TIMER1_COMPA_vect) {
    switch(_rtx_state) {
        case STATUS_IDLE:
            // We've finished transmitting the previous byte.
            if(_rtx_head == _rtx_tail) {
                break;
            }
            // fetch the next byte to sen.
            _rtx_byte = _rtx_tx[_rtx_tail];
            _rtx_tail = (_rtx_tail + 1) % RTX_BUFFER_SIZE;
            _rtx_counter = 0;
            // Fall through to send the stop bit.
        case STATUS_START:
            // Send the start bit.
            cbi(PORTB, RTX_PIN);
            _rtx_state = STATUS_DATABIT;
            break;
        case STATUS_DATABIT:
            // Send the next databit if not all 8 have been sent yet.
            if((_rtx_byte >> _rtx_counter) & 0x01)
                sbi(PORTB, RTX_PIN);
            else
                cbi(PORTB, RTX_PIN);
            // Done with the byte, send the stop bits.
            if(++_rtx_counter >= 8) {
                _rtx_state = STATUS_STOP;
                _rtx_counter = 0;
            }
            break;
        case STATUS_STOP:
            // Send a stop bit if two haven't been sent yet
            sbi(PORTB, RTX_PIN);
            if(++_rtx_counter >= 2) {
                _rtx_state = STATUS_IDLE;
            }
            break;
    }
}

void rtx_start(uint16_t bps) {
    _rtx_head = _rtx_tail = 0;
    
    sbi(DDRB, RTX_PIN);
    sbi(PORTB, RTX_PIN);
    
    cli();
    // disable interrupts while we setup the timer
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = F_CPU / 1024 / bps - 1;
    sbi(TCCR1B, WGM12);
    sbi(TCCR1B, CS10);
    sbi(TCCR1B, CS12);
    sbi(TIMSK1, OCIE1A);
    // enable interrupts.
    sei();
}

void rtx_stop() {
    cli();
    cbi(TIMSK1, OCIE1A);
    sei();
}

bool rtx_ready() {
    return (RTX_BUFFER_SIZE + _rtx_head - _rtx_tail) % RTX_BUFFER_SIZE;
}

int8_t rtx_write(byte_t data) {
    index_t next_head = (_rtx_head + 1) % RTX_BUFFER_SIZE;
    // block until we can put the data byte on the buffer.
    while(next_head == _rtx_tail){}
    _rtx_tx[_rtx_head] = data;
    _rtx_head = next_head;
    return 1;
}

index_t rtx_write_bytes(const byte_t* data, index_t length) {
    index_t w;
    for(w = 0; w < length; ++w) {
        if(rtx_write(*(data++)) != 1) {
            break;
        }
    }
    return w;
}
