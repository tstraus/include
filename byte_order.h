#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__

template <typename T>
static inline T hton(const T& input)
{
    int check = 42;
    if (*(char*) &check == 42)
    {
        T output(0);
        const std::size_t size = sizeof(input) - 1;
        uint8_t* data = reinterpret_cast<uint8_t*>(&output);

        for (std::size_t i = 0; i <= size; i++)
            data[i] = (uint8_t)(input >> ((size - i) * 8));

        return output;
    } else {
        return input;
    }
}

template <typename T>
static inline T ntoh(const T& input)
{
    return hton<T>(input); // all it does is reverse bytes if it has too, there is no real nh directionality
}

static inline float htonf(const float& input)
{
    int check = 42;
    if (*(char*)&check == 42)
    {
        uint32_t output = ntoh<uint32_t>(*(uint32_t*)&input);
        return *(float*)&output;
    }
    else {
        return input;
    }
}

static inline float ntohf(const float& input)
{
    return htonf(input);
}

static inline double htond(const double& input)
{
    int check = 42;
    if (*(char*)&check == 42)
    {
        uint64_t output = ntoh<uint64_t>(*(uint64_t*)&input);
        return *(double*)&output;
    }
    else {
        return input;
    }
}

static inline double ntohd(const double& input)
{
    return htond(input);
}

#endif // __BYTE_ORDER_H__