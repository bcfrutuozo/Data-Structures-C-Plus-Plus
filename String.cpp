//
// Created by bcfrutuozo on 12/03/2022.
//

#include "String.h"
#include "Queue.h"

#include <iostream>
#include <string>
#include <cstdio>

String::String() noexcept
        :
        m_Data(nullptr),
        m_Length(0) {
    m_Data = new char[1];
    m_Data[0] = '\0';
}

String::String(const char *c) noexcept
        :
        m_Data(nullptr) {
    if (c == nullptr) {
        m_Data = new char[1];
        m_Data[0] = '\0';
        m_Length = 0;
    } else {
        size_t size = strlen(c);
        m_Data = new char[size + 1];
        strcpy(m_Data, c);
        m_Data[size] = '\0';
        m_Length = size;
    }
}

String::String(const String &s) noexcept {
    size_t size = strlen(s.m_Data);
    m_Data = new char[size + 1];
    strcpy(m_Data, s.m_Data);
    m_Data[size] = '\0';
    m_Length = size;
}

String::String(String &&s) noexcept {
    m_Data = s.m_Data;
    m_Length = s.m_Length;
    s.m_Data = nullptr;
}

String::~String() noexcept {
    delete[] m_Data;
    m_Data = nullptr;
}

String &String::operator=(const char *c) noexcept {
    if (&m_Data == &c) {
        return *this;
    }

    delete[] m_Data;
    size_t newSize = strlen(c);
    m_Data = new char[newSize + 1];
    strcpy(m_Data, c);
    m_Length = newSize;
    return *this;
}

String &String::operator=(const String &s) noexcept {
    if (this == &s) {
        return *this;
    }

    delete[] m_Data;
    size_t newSize = strlen(s.m_Data);
    m_Data = new char[newSize + 1];
    strcpy(m_Data, s.m_Data);
    m_Length = newSize;
    return *this;
}

String &String::operator=(String &&other) noexcept {

    // Self-assignment detection
    if (&other == this) return *this;

    delete[] m_Data;

    size_t newSize = other.m_Length;
    m_Data = other.m_Data;

    other.m_Length = 0;
    m_Data = nullptr;

    return *this;
}

String String::operator+(const char *c) noexcept {
    size_t newSize = m_Length + strlen(c);
    char *buffer = new char[newSize + 1];
    strcpy(buffer, m_Data);
    strcat(buffer, c);
    buffer[newSize] = '\0';
    String temp{buffer};
    delete[] buffer;
    return temp;
}

String String::operator+(const String &s) noexcept {
    size_t newSize = m_Length + s.m_Length;
    char *buffer = new char[newSize + 1];
    strcpy(buffer, m_Data);
    strcat(buffer, s.m_Data);
    buffer[newSize] = '\0';
    String temp{buffer};
    delete[] buffer;
    return temp;
}

String& String::operator+=(const char* c) noexcept {
    strcat(this->m_Data, c);
    m_Length = strlen(this->m_Data);
    return *this;
}

inline String& String::operator+=(const String& rhs) noexcept {
    return *this += rhs.m_Data;
}

char String::operator[](size_t index) {
    if (index > m_Length - 1) throw std::out_of_range("Out of string boundaries");
    return m_Data[index];
}

bool String::operator==(const char *c) const noexcept {
    if (m_Length != strlen(c))
        return false;

    return strcmp(m_Data, c) == 0;
}

inline bool String::operator==(const String &rhs) const noexcept {
    return *this == rhs.m_Data;
}

inline bool String::operator!=(const char* c) const noexcept{
    return !(*this == c);
}

inline bool String::operator!=(const String &rhs) const noexcept{
    return !(*this == rhs);
}

bool String::Contains(const char c) const noexcept {
    for (size_t i = 0; i < m_Length; ++i)
        if (*(m_Data + i) == c)
            return true;

    return false;
}

bool String::Contains(const char *c) const noexcept {

    size_t cSize = strlen(c);

    if (cSize > m_Length) return false;

    auto lastBlock = reinterpret_cast<size_t>(m_Data + m_Length);

    for (size_t i = 0; i < m_Length && reinterpret_cast<size_t>(m_Data + i) <= lastBlock; ++i) {

        //ifdef IS_DEBUG
        std::cout << "Comparing ";

        for (size_t start = i; start < i + cSize; ++start)
            std::cout << *(m_Data + start);

        std::cout << " with " << c << std::endl;
        //#endif

        if (memcmp((m_Data + i), c, cSize) == 0)
            return true;
    }

    return false;
}

void String::Copy(char *c, size_t length, size_t pos) {
    for (size_t i = 0; i < length; ++i) {
        c[i] = m_Data[pos + i];
    }
    c[length] = '\0';
}

size_t String::Count(char c) const noexcept {
    if (GetLength() == 0) return 0;

    auto p = m_Data;
    size_t count = 0;
    while (*p != '\0') {
        if (*p == c) ++count;
        ++p;
    }

    return count;
}

size_t String::Count(const char *c) const noexcept {
    if (GetLength() == 0) return 0;

    return IndicesOf(c).GetLength();
}

ssize_t String::IndexOf(const char c) const noexcept {
    if (GetLength() == 0) return -1;

    return (ssize_t) (strchr(m_Data, c) - m_Data);
}

ssize_t String::IndexOf(const char *c) const noexcept {
    if (GetLength() == 0) return -1;

    return (ssize_t) (strstr(m_Data, c) - m_Data);
}

Array<size_t> String::IndicesOf(const char c) const noexcept {
    if (GetLength() == 0) return Array<size_t>{};

    Queue<size_t> q;
    for (size_t i = 0;; ++i) {
        auto a = (strchr((char *) m_Data + i, c));
        if (a != nullptr) {
            auto idx = a - m_Data;
            q.Enqueue(idx);
            i = idx;
        } else break;
    }

    return q.ToArray();
}

Array<size_t> String::IndicesOf(const char *c) const noexcept {
    if (GetLength() == 0) return Array<size_t>{};

    Queue<size_t> q;
    const size_t inc = strlen(c);
    for (auto p = strstr(m_Data, c); p != nullptr; p = strstr(p + inc, c))
        q.Enqueue(p - m_Data);

    return q.ToArray();
}

Array<String> String::InternalSplit(const char* c, int count, Array<size_t>& array, StringSplitOptions options) const noexcept {

    if(count > array.GetLength()) count = array.GetLength();

    size_t length = strlen(c);

    //Handle the special case of no replaces and special count.
    if (array.GetLength() == 0) {
        return Array<String>{m_Data};
    }

    Queue<String> q;

    size_t start = 0;
    size_t end;
    size_t charSize;

    switch (options) {
        case StringSplitOptions::RemoveEmptyEntries:
            if (array[0] != 0)
            {
                if (count > 1)
                    end = array[0] - 1;
                else
                    end = GetLength();
                charSize = end - start;
                char *buffer = new char[charSize + 1];
                std::copy(m_Data + start, m_Data + end + 1, buffer);
                *(buffer + charSize + 1) = '\0';
                q.Enqueue(buffer);
                delete[] buffer;
            }

            for (ssize_t i = 0; i < count; ++i) {

                start = array[i] + length;

                if (count - 1 > i)
                    end = array[i + 1] - 1;
                else
                    end = GetLength();

                if (start <= end) {
                    charSize = end - start;
                    char *buffer = new char[charSize + 1];
                    std::copy(m_Data + start, m_Data + end + 1, buffer);
                    *(buffer + charSize + 1) = '\0';
                    q.Enqueue(buffer);
                    delete[] buffer;
                }
            }

            break;
        default:

            if (array[0] == 0)
                q.Enqueue(String{});
            else {
                if (count > 1)
                    end = array[0] - 1;
                else
                    end = GetLength();
                charSize = end - start;
                char *buffer = new char[charSize + 1];
                std::copy(m_Data + start, m_Data + end + 1, buffer);
                *(buffer + charSize + 1) = '\0';
                q.Enqueue(buffer);
                delete[] buffer;
            }

            for (ssize_t i = 0; i < count; ++i) {

                start = array[i] + length;

                if (count - 1 > i)
                    end = array[i + 1] - 1;
                else
                    end = GetLength();

                if (start > end) q.Enqueue(String{});
                else {
                    charSize = end - start;
                    char *buffer = new char[charSize + 1];
                    std::copy(m_Data + start, m_Data + end + 1, buffer);
                    *(buffer + charSize + 1) = '\0';
                    q.Enqueue(buffer);
                    delete[] buffer;
                }
            }

            break;
    }

    return q.ToArray();
}

String String::Join(char separator, Array<String>& arrayString) noexcept{
    char temp[2] = { separator, '\0'};
    return String::Join(temp, arrayString);
}

String String::Join(char separator, Array<String>&& arrayString) noexcept{
    char temp[2] = { separator, '\0'};
    return String::Join(temp, arrayString);
}

String String::Join(const char* separator, Array<String>& arrayString) noexcept {

    if(arrayString.GetLength() == 0) return String{};

    size_t totalSize = 0;
    for(const auto& x : arrayString)
        totalSize += x.GetLength();

    totalSize += arrayString.GetLength();

    char* buffer = new char[totalSize];
    size_t start = 0;
    size_t end = 0;
    size_t length = strlen(separator);

    for(size_t i = 0 ; i < arrayString.GetLength(); ++i){
        String stg = arrayString[i];
        start = i == 0 ? 0 : end + length;
        end = i == 0 ? stg.GetLength() : start + stg.GetLength();

        std::copy(stg.GetPointer(), stg.GetPointer() + stg.GetLength(), buffer + start);

        if(i == arrayString.GetLength() - 1)
            *(buffer + end) = '\0';
        else
            std::copy(separator, separator + length, buffer + end);
    }

    String cat = buffer;
    delete[] buffer;

    return cat;
}

String String::Join(const char* separator, Array<String>&& arrayString) noexcept {
    return String::Join(separator, arrayString);
}

ssize_t String::LastIndexOf(const char c) const noexcept {
    if (GetLength() == 0) return -1;

    return (ssize_t) (strrchr(m_Data, c) - m_Data);
}

ssize_t String::LastIndexOf(const char *c) const noexcept {
    if (GetLength() == 0) return -1;

    char *haystack = m_Data;
    char *r = nullptr;

    if (!c[0])
        return (ssize_t) (haystack + strlen(haystack));
    while (true) {
        char *p = strstr(haystack, c);
        if (!p)
            return (ssize_t) (r - m_Data);
        r = p;
        haystack = p + 1;
    }
}

Array<String> String::Split(char delimiter, int count) {
    if(count < 0) throw std::invalid_argument("count");

    if ((GetLength() == 0)) {
        return Array<String>{m_Data};
    }

    auto array = IndicesOf(delimiter);
    char temp[2] = { delimiter, '\0'} ;
    return InternalSplit(temp, count, array);
}

Array<String> String::Split(const char* delimiter, int count){
    if(count < 0) throw std::invalid_argument("count");

    if ((GetLength() == 0)) {
        return Array<String>{m_Data};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(delimiter, count, array);
}

Array<String> String::Split(char delimiter, StringSplitOptions options) const noexcept {
    if (GetLength() == 0) {
        if(options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{m_Data};
    }

    auto array = IndicesOf(delimiter);
    char temp[2] = { delimiter, '\0'} ;
    return InternalSplit(temp, array.GetLength(), array, options);
}

Array<String> String::Split(const char *delimiter, StringSplitOptions options) const noexcept {
    if (GetLength() == 0) {
        if(options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{m_Data};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(delimiter, array.GetLength(), array, options);
}

Array<String> String::Split(char delimiter, int count, StringSplitOptions options){
    if(count < 0) throw std::invalid_argument("count");

    if (GetLength() == 0) {
        if(options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{m_Data};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(&delimiter, count, array, options);
}

Array<String> String::Split(const char* delimiter, int count, StringSplitOptions options){
    if(count < 0) throw std::invalid_argument("count");

    if (GetLength() == 0) {
        if(options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{m_Data};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(delimiter, count, array, options);
}

String String::Trim() const noexcept {
    return Trim(Space);
}

String String::Trim(char c) const noexcept {
    if (GetLength() == 0) return "";

    size_t start = 0;

    // Getting start pointer
    while (*(m_Data + start) == c) ++start;

    auto end = (size_t) (GetLength() - 1); // 1 for \0 and 1 for index

    while (*(m_Data + end) == c) {
        --end;
    }

    char *buffer = new char[end - start + 2];
    std::copy(m_Data + start, m_Data + end + 1, buffer);
    buffer[end - start + 1] = '\0';
    String ret = buffer;
    delete[] buffer;

    return ret;
}

String String::Trim(const char *c) const noexcept {
    if (GetLength() == 0) return "";

    return "";
}

String String::TrimEnd() const noexcept {
    return TrimEnd(Space);
}

String String::TrimEnd(char c) const noexcept {
    if (GetLength() == 0) return "";

    size_t end = GetLength() - 1;

    while (*(m_Data + end) == c) {
        --end;
    }

    char *buffer = new char[end + 1];
    std::copy(m_Data, m_Data + end + 1, buffer);
    buffer[end + 1] = '\0';
    String ret = buffer;
    delete[] buffer;

    return ret;
}

String String::TrimEnd(const char *c) const noexcept {
    return "";
}

String String::TrimStart() const noexcept {
    return TrimStart(Space);
}

String String::TrimStart(char c) const noexcept {
    if (GetLength() == 0) return "";

    auto p = m_Data;
    size_t start = 0;

    while (*p == c) {
        ++start;
        ++p;
    }

    return m_Data + start;
}

String String::TrimStart(const char *c) const noexcept {
    if (GetLength() == 0) return "";

    auto index = strstr(m_Data, c) - m_Data;
    if (index != 0) return m_Data;

    return m_Data + strlen(c);
}
