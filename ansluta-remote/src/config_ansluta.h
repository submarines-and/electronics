/*
    Go to web interface: https://192.168.176.175/ansluta/getAddress
    press button on your original ansluta remote within 5 sec
    If wiring is correct, the bytes should be displayed in web interface (and terminal if monitoring)

    In web interface, you will get 4 chars. Split them into to and add 0x as prefix. E.g. the below values were shown as 88c7 in web interface.
*/

#define ADDRESS_BYTE_A 0x88
#define ADDRESS_BYTE_B 0xc7