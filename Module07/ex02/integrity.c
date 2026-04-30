#include "libalex_avr.h"

uint16_t	makeChecksum16(node_t *node)
{
    uint32_t sum = 0;
    uint8_t *bytes = (uint8_t *)node;
    
    for (uint8_t byteAddress = 0; byteAddress < NODE_SIZE_WITHOUT_INTEGRITY_CHECK; byteAddress++)
    {
        uart_printhex(bytes[byteAddress]);
        sum += bytes[byteAddress];
    }

    return ((uint16_t)(sum & 0xFFFF0000 >> 16) + (uint16_t)(sum & 0x0000FFFF));
}

int	verifyChecksum16(node_t *node)
{
    return (makeChecksum16(node) == node->integrityCheck);
}