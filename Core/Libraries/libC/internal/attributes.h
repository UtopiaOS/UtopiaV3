


#define UTOPIA_ENUM(type, name, prefix) \
        typedef type prefix ## type_ ## name; \
        enum name

#define UTOPIA_OPTIONS(type, name, prefix) \
    typedef type prefix ## type_ ## name; \
    enum __attribute__((flag_enum)) name


#define UTOPIA_ALWAYS_INLINE static __attribute__((always_inline))


// While this code is mostly (or completely) copied/borrowed by Anillo-OS by facekapow, packaging structs was something I didn't
// know about prior to starting the Development of Utopia. They are truly a lost art.
// You can read more about them in this StackOverflow post: https://stackoverflow.com/questions/4306186/structure-padding-and-packing
// And this book: http://www.catb.org/esr/structure-packing/
#define UTOPIA_PACKED_STRUCT(name, prefix) \
    typedef struct __attribute__((packed)) name prefix ## type_ ## name; \
    struct __attribute__((packed)) name