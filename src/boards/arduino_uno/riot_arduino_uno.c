#include "riot_arduino_uno.h"

static const uint8_t PIN_D0 = 0;
static const uint8_t PIN_D1 = 1;
static const uint8_t PIN_D2 = 2;
static const uint8_t PIN_D3 = 3;
static const uint8_t PIN_D4 = 4;
static const uint8_t PIN_D5 = 5;
static const uint8_t PIN_D6 = 6;
static const uint8_t PIN_D7 = 7;
static const uint8_t PIN_D8 = 8;
static const uint8_t PIN_D9 = 9;
static const uint8_t PIN_D10 = 10;
static const uint8_t PIN_D11 = 11;
static const uint8_t PIN_D12 = 12;
static const uint8_t PIN_D13 = 13;

typedef struct write_to_arduino_uno_stream_of_byte_t {
	void(*next)(stream_of_byte *self, byte v);
	void(*error)(stream_of_byte *self, byte e);
	void(*complete)(stream_of_byte *self);
	varray *listeners;
  uint8_t pin;
	void(*write_to_arduino_uno)(byte value);
} write_to_arduino_uno_stream_of_byte;

void riot_arduino_uno_write(uint8_t pin, byte value) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

void write_to_arduino_uno_stream_of_byte_next(stream_of_byte *stream, byte value) {
	write_to_arduino_uno_stream_of_byte* write_stream = (write_to_arduino_uno_stream_of_byte*)stream;
	uint8_t pin = write_stream->pin;
  riot_arduino_uno_write(pin, value);
}

stream_of_byte* riot_arduino_uno_write_listener_create(uint8_t pin) {
  write_to_arduino_uno_stream_of_byte* stream = xmalloc(sizeof(write_to_arduino_uno_stream_of_byte));
	stream_of_byte_init(stream);
	stream->pin = pin;
	stream->next = write_to_arduino_uno_stream_of_byte_next;
	return (stream_of_byte*) stream;
}

riot_arduino_uno_sinks* riot_arduino_uno_sinks_create() {
  riot_arduino_uno_sinks *sinks = xmalloc(sizeof(riot_arduino_uno_sinks));
  sinks->D0 = riot_arduino_uno_write_listener_create(PIN_D0);
  sinks->D1 = riot_arduino_uno_write_listener_create(PIN_D1);
  sinks->D2 = riot_arduino_uno_write_listener_create(PIN_D2);
  sinks->D3 = riot_arduino_uno_write_listener_create(PIN_D3);
  sinks->D4 = riot_arduino_uno_write_listener_create(PIN_D4);
  sinks->D5 = riot_arduino_uno_write_listener_create(PIN_D5);
  sinks->D6 = riot_arduino_uno_write_listener_create(PIN_D6);
  sinks->D7 = riot_arduino_uno_write_listener_create(PIN_D7);
  sinks->D8 = riot_arduino_uno_write_listener_create(PIN_D8);
  sinks->D9 = riot_arduino_uno_write_listener_create(PIN_D9);
  sinks->D10 = riot_arduino_uno_write_listener_create(PIN_D10);
  sinks->D11 = riot_arduino_uno_write_listener_create(PIN_D11);
  sinks->D12 = riot_arduino_uno_write_listener_create(PIN_D12);
  sinks->D13 = riot_arduino_uno_write_listener_create(PIN_D13);
  sinks->LED = sinks->D13;
  return sinks;
}

riot_arduino_uno_sources* riot_arduino_uno_sources_create() {
  riot_arduino_uno_sources *sources = xmalloc(sizeof(riot_arduino_uno_sources));
  sources->D0 = stream_of_byte_create();
  sources->D1 = stream_of_byte_create();
  sources->D2 = stream_of_byte_create();
  sources->D3 = stream_of_byte_create();
  sources->D4 = stream_of_byte_create();
  sources->D5 = stream_of_byte_create();
  sources->D6 = stream_of_byte_create();
  sources->D7 = stream_of_byte_create();
  sources->D8 = stream_of_byte_create();
  sources->D9 = stream_of_byte_create();
  sources->D10 = stream_of_byte_create();
  sources->D11 = stream_of_byte_create();
  sources->D12 = stream_of_byte_create();
  sources->D13 = stream_of_byte_create();
  sources->LED = sources->D13;
  return sources;
};

/**
  We read the pins and manually push a next value to the individual
  streams in the sources struct.
*/
void riot_arduino_uno_read_inputs(riot_arduino_uno_sources *sources) {
  pinMode(PIN_D0, INPUT);
  sources->D0->next(sources->D0, digitalRead(PIN_D0));
  pinMode(PIN_D1, INPUT);
  sources->D1->next(sources->D1, digitalRead(PIN_D1));
  pinMode(PIN_D2, INPUT);
  sources->D2->next(sources->D2, digitalRead(PIN_D2));
  pinMode(PIN_D3, INPUT);
  sources->D3->next(sources->D3, digitalRead(PIN_D3));
  pinMode(PIN_D4, INPUT);
  sources->D4->next(sources->D4, digitalRead(PIN_D4));
  pinMode(PIN_D5, INPUT);
  sources->D5->next(sources->D5, digitalRead(PIN_D5));
  pinMode(PIN_D6, INPUT);
  sources->D6->next(sources->D6, digitalRead(PIN_D6));
  pinMode(PIN_D7, INPUT);
  sources->D7->next(sources->D7, digitalRead(PIN_D7));
  pinMode(PIN_D8, INPUT);
  sources->D8->next(sources->D8, digitalRead(PIN_D8));
  pinMode(PIN_D9, INPUT);
  sources->D9->next(sources->D9, digitalRead(PIN_D9));
  pinMode(PIN_D10, INPUT);
  sources->D10->next(sources->D10, digitalRead(PIN_D10));
  pinMode(PIN_D11, INPUT);
  sources->D11->next(sources->D11, digitalRead(PIN_D11));
  pinMode(PIN_D12, INPUT);
  sources->D12->next(sources->D12, digitalRead(PIN_D12));
  pinMode(PIN_D13, INPUT);
  sources->D13->next(sources->D13, digitalRead(PIN_D13));
}

/**
  Returns true if the sinks is empty, i.e., all the streams in the
  sinks struct point to null. 
*/
bool riot_arduino_uno_is_sinks_empty(riot_arduino_uno_sinks *sinks) {
  if(sinks->D0 != NULL) return false;
  if(sinks->D1 != NULL) return false;
  if(sinks->D2 != NULL) return false;
  if(sinks->D3 != NULL) return false;
  if(sinks->D4 != NULL) return false;
  if(sinks->D5 != NULL) return false;
  if(sinks->D6 != NULL) return false;
  if(sinks->D7 != NULL) return false;
  if(sinks->D8 != NULL) return false;
  if(sinks->D9 != NULL) return false;
  if(sinks->D10 != NULL) return false;
  if(sinks->D11 != NULL) return false;
  if(sinks->D12 != NULL) return false;
  if(sinks->D13 != NULL) return false;
  if(sinks->A0 != NULL) return false;
  if(sinks->A1 != NULL) return false;
  if(sinks->A2 != NULL) return false;
  if(sinks->A3 != NULL) return false;
  if(sinks->A4 != NULL) return false;
  if(sinks->A5 != NULL) return false;
  return true;
}

int riot_arduino_uno_run(riot_arduino_uno_main main) {
  if(main == NULL) return -1;
  riot_arduino_uno_sources *sources = riot_arduino_uno_sources_create();
  if(sources == NULL) return -2;
  riot_arduino_uno_sinks *sinks = main(sources);
  if(sinks == NULL) return -3;
  while (true) {
    riot_arduino_uno_read_inputs(sources);
    if(riot_arduino_uno_is_sinks_empty(sinks))
      break;
  }
  return 0;
}
