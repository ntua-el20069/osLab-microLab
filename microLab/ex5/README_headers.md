## Use of Header files in MPLAB X

You can make a project like this
### ask_x
- Header Files (containing `twi_api.h`)
- Source Files (containing `twi_api.c` for twi communication and `ask_x.c` for main program and new functions)

Don't forget to include the header file inside your main code
```c
#include "twi_api.h"
```
