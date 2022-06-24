


#define UTOPIA_ENUM(type, name, prefix) \
    typedef type prefix ## type_ ## name; \
    enum 

#define UTOPIA_OPTIONS(type, name, prefix) \
    typedef type prefix ## type_ ## name; \
    enum __attribute__((flag_enum)) name


#define UTOPIA_ALWAYS_INLINE static __attribute__((always_inline))

