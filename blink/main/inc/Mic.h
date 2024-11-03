#include "driver/i2s.h"
//#include "driver/i2s_std.h"
#include "freertos/queue.h"

class Mic
{
public:

    Mic();

    void init();



private:

    i2s_config_t m_i2sConfig; 

    i2s_pin_config_t m_i2sPins;





};