#define DISC_ 0xf601
#define CONNECT_ 0xf602
#define COLOR_ 0xf603
#define MOVE_ 0xf604
#define ACK_ 0xf605
#define NACK_ 0xf606

#include <oc_stdio.h>
#include <oc_uart.h>
#include <oc_pins.h>

typedef void(*_callback)(void*, size_t);

typedef struct
{
    uint16_t cmd;
    char data[64];
} MyStruct_t;

static oC_UART_Context_t context = NULL;
static oC_UART_Config_t config = {
        .Rx         = oC_Pin_PC6,                       // The RX pin
        .Tx         = oC_Pin_PC7,                       // The TX pin
        .WordLength = oC_UART_WordLength_8Bits,         // 8 bits per word
        .BitRate    = 9600,                             // 9600 bits per second
        .Parity     = oC_UART_Parity_None,              // No parity
        .StopBit    = oC_UART_StopBit_1Bit,             // 1 stop bit
        .BitOrder   = oC_UART_BitOrder_LSBFirst,        // Least significant bit first
        .Invert     = oC_UART_Invert_NotInverted,       // Not inverted signals
        .Dma        = oC_UART_Dma_DontUse,              // Don't use DMA
        .Loopback   = false                             // Loopback mode for testing
};

// static bool ReadPin(const char* PinName, oC_Pin_t* Pin )
// {
//     oC_ErrorCode_t errorCode = oC_ErrorCode_ImplementError;
//     if( ErrorCode( oC_GPIO_FindPinByName(PinName, Pin) ) )
//     {
//         return true;
//     }
//     else
//     {
//         printf("Cannot find the pin '%s': %R\n", PinName, errorCode);
//         return false;
//     }
// }


static bool ConfigureUart( oC_UART_Config_t* Config, oC_UART_Context_t* Context )
{
    oC_ErrorCode_t errorCode = oC_ErrorCode_ImplementError;
    printf("Configuring the UART...\n");

    // Call the configuration function
    if( ErrorCode( oC_UART_Configure(Config, Context) ) )
    {
        return true;
    }
    else
    {
        printf("Configuration failed: %R\n", errorCode);
        return false;
    }
}

static bool Unconfigure( oC_UART_Context_t* Context , oC_UART_Config_t* Config )
{
    oC_ErrorCode_t errorCode = oC_ErrorCode_ImplementError;

    printf("Unconfiguration of the UART driver\n");
    errorCode = oC_UART_Unconfigure(Config, Context);
    if(errorCode != oC_ErrorCode_None)
    {
        printf("Error while unconfiguring the UART: %R\n", errorCode);
        return false;
    }

    return true;
}

static bool Transmit( oC_UART_Context_t Context , const char* Data, int dataSize)
{
    oC_ErrorCode_t errorCode = oC_ErrorCode_ImplementError;
    oC_MemorySize_t size = dataSize;

    printf("Sending data: %s\n", Data);
    errorCode = oC_UART_Write(Context, Data, &size, oC_s(1));
    if(errorCode != oC_ErrorCode_None)
    {
        printf("Error while sending data: %R\n", errorCode);
        return false;
    }

    return true;
}

static bool Receive( oC_UART_Context_t Context , char* Buffer, oC_MemorySize_t Size )
{
    oC_ErrorCode_t errorCode = oC_ErrorCode_ImplementError;

    errorCode = oC_UART_Receive(Context, Buffer, Size, oC_s(1));

    if( errorCode != oC_ErrorCode_None && errorCode != oC_ErrorCode_NoAllBytesRead )
    {
        printf("Error while receiving data: %R\n", errorCode);
        return false;
    }

    printf("Received data: %s\n", Buffer);

    return true;
}

void testowy_callback(void* data, size_t dataSize)
{
    printf("I have data!");
}

static bool startUart()
{
    if(!ConfigureUart(&config, &context))
    {
        printf("Can not init uart");
        return false;
    }

    return true;
}

bool endUart()
{
    Unconfigure(&context, &config);
    return true;
}

bool readState(_callback dataReceiveCallback)
{
    MyStruct_t frame, ack;
    memset(&frame, 0, sizeof(MyStruct_t));
    memset(&ack, 0, sizeof(MyStruct_t));
    ack.cmd = ACK_;
    while(!Receive(context, (char*)&frame, sizeof(frame)));
    Transmit(context, (char*)&ack, sizeof(frame));
    dataReceiveCallback((void*)&frame.data, sizeof(frame.data));
    return true;
}

bool sendState(void* data, size_t dataSize)
{
    MyStruct_t frame, ack;
    memset(&frame, 0, sizeof(MyStruct_t));
    frame.cmd = MOVE_;
    memcpy(&frame.data, data, dataSize);
    Transmit(context, (char*)&frame, sizeof(frame));
    while(!Receive(context, (char*)&ack, sizeof(ack)));
    return true;
}
