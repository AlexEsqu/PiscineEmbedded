#include "libalex_avr.h"

#include <stddef.h>

uint16_t	makeChecksum16(node_t *node)
{
    uint32_t sum = 0;
    const uint8_t *bytes = (const uint8_t *)node;

    for (uint8_t i = 0; i < NODE_SIZE_WITHOUT_INTEGRITY_CHECK; ++i)
    {
        sum += bytes[i];
    }

    sum = (sum & 0xFFFF) + (sum >> 16);
    sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)sum;
}

int	verifyChecksum16(node_t *node)
{
    return (makeChecksum16(node) == node->integrityCheck);
}