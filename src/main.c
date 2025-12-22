#include "log.h"
#include "app_init.h"

#define TAG  "Main"

#if !defined(UNIT_TEST)
void app_main(void)
{
    LOGI(TAG, "Starting application");

    app_init();
}
#endif  // UNIT_TEST
