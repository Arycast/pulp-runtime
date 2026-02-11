#include <stdbool.h>
#include <stddef.h>

#include "variables.h"
#include "String.hpp"

#include "IPAddress.h"
#include "wifi_client.h"

WiFiClient client;

WiFiClient::WiFiClient(void)
{
}

WiFiClient::~WiFiClient(void)
{
}

bool WiFiClient::connected(void) {
	return -1;
}

bool WiFiClient::connect(IPAddress ip, int port) {
	return -1;
}

bool WiFiClient::connect(String URL, int port) {
	return -1;
}

size_t WiFiClient::write(byte data) {
	return -1;
}

size_t WiFiClient::write(char data) {
	return -1;
}

size_t WiFiClient::write(const byte *data, size_t length)
{
	return -1;
}

/*size_t WiFiClient::write(const char *data, size_t length)
{
	return -1;
}*/


int WiFiClient::available(void)
{
	return -1;
}

int WiFiClient::read(void)
{
	return -1;
}

void WiFiClient::flush(void)
{
}

int WiFiClient::peek(void)
{
	return -1;
}

byte WiFiClient::print(char data) {
	return -1;
}

byte WiFiClient::print(byte data) {
	return -1;
}

byte WiFiClient::print(int data) {
	return -1;
}

byte WiFiClient::print(long data) {
	return -1;
}

byte WiFiClient::print(String data) {
	return -1;
}

byte WiFiClient::print(char data, int BASE) {
	return -1;
} 

byte WiFiClient::print(byte data, int BASE) {
	return -1;
}

byte WiFiClient::print(int data, int BASE) {
	return -1;
}

byte WiFiClient::print(long data, int BASE) {
	return -1;
}

byte WiFiClient::print(String data, int BASE) {
	return -1;
}

byte WiFiClient::println(void) {
	return -1;
}

byte WiFiClient::println(char data) {
	return -1;
}

byte WiFiClient::println(byte data) {
	return -1;
}

byte WiFiClient::println(int data) {
	return -1;
}

byte WiFiClient::println(long data) {
	return -1;
}

byte WiFiClient::println(String data) {
	return -1;
}

byte WiFiClient::println(char data, int BASE) {
	return -1;
}

byte WiFiClient::println(byte data, int BASE) {
	return -1;
}

byte WiFiClient::println(int data, int BASE) {
	return -1;
}

byte WiFiClient::println(long data, int BASE) {
	return -1;
}

byte WiFiClient::println(String data, int BASE) {
	return -1;
}

void WiFiClient::stop(void) {

}