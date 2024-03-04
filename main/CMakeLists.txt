set(srcs "main.c"
         "./bluetooth/gatt_server.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ".")
