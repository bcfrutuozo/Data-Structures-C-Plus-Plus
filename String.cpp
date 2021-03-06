//
// Created by bcfrutuozo on 12/03/2022.
//

#include "String.h"
#include "Queue.h"

#include <iostream>
#include <cstdio>
#include <string.h>

String::String() noexcept
        :
        m_Data(nullptr),
        m_Length(0) {
    m_Data = new Char[1];
    m_Data[0] = '\0';
}

String::String(char c, int count) noexcept
    :
    m_Length(count) {
    m_Data = new Char[count + 1];
    for(int i = 0; i < count; ++i) m_Data[i] = c;
    m_Data[count] = '\0';
}

String::String(const char *c) noexcept
        :
        m_Data(nullptr) {
    if (c == nullptr) {
        m_Data = new Char[1];
        m_Data[0] = '\0';
        m_Length = 0;
    } else {
        size_t size = strlen(c);
        m_Data = new Char[size + 1];
        strcpy(reinterpret_cast<char*>(m_Data), c);
        m_Data[size] = '\0';
        m_Length = size;
    }
}

String::String(const String &s) noexcept {
    size_t size = strlen(reinterpret_cast<char*>(s.m_Data));
    m_Data = new Char[size + 1];
    strcpy(reinterpret_cast<char*>(m_Data), reinterpret_cast<char*>(s.m_Data));
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
    if (reinterpret_cast<const char*>(m_Data) == c) {
        return *this;
    }

    delete[] m_Data;
    size_t newSize = strlen(c);
    m_Data = new Char[newSize + 1];
    strcpy(reinterpret_cast<char*>(m_Data), c);
    m_Length = newSize;
    return *this;
}

String &String::operator=(const String &s) noexcept {
    if (this == &s) {
        return *this;
    }

    delete[] m_Data;
    size_t newSize = strlen(reinterpret_cast<char*>(s.m_Data));
    m_Data = new Char[newSize + 1];
    strcpy(reinterpret_cast<char*>(m_Data), reinterpret_cast<char*>(s.m_Data));
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
    strcpy(buffer, reinterpret_cast<char*>(m_Data));
    strcat(buffer, c);
    buffer[newSize] = '\0';
    String temp{buffer};
    delete[] buffer;
    return temp;
}

String String::operator+(const String &s) noexcept {
    size_t newSize = m_Length + s.m_Length;
    char *buffer = new char[newSize + 1];
    strcpy(buffer, reinterpret_cast<char*>(m_Data));
    strcat(buffer, reinterpret_cast<char*>(s.m_Data));
    buffer[newSize] = '\0';
    String temp{buffer};
    delete[] buffer;
    return temp;
}

String &String::operator+=(const char *c) noexcept {
    strcat(reinterpret_cast<char*>(this->m_Data), c);
    m_Length = strlen(reinterpret_cast<char*>(this->m_Data));
    return *this;
}

inline String &String::operator+=(const String &rhs) noexcept {
    return *this += reinterpret_cast<char*>(rhs.m_Data);
}

Boolean String::operator==(const char *c) const noexcept {
    if (m_Length != strlen(c))
        return false;

    return strcmp(reinterpret_cast<char*>(m_Data), c) == 0;
}

inline Boolean String::operator==(const String &rhs) const noexcept {
    return *this == reinterpret_cast<char*>(rhs.m_Data);
}

inline Boolean String::operator!=(const char *c) const noexcept {
    return !(*this == c);
}

inline Boolean String::operator!=(const String &rhs) const noexcept {
    return !(*this == rhs);
}

Boolean String::Contains(const char c) const noexcept {
    auto p = strchr(reinterpret_cast<char*>(m_Data), c);
    if (p == nullptr) return false;
    return true;
}

Boolean String::Contains(const char *c) const noexcept {

    auto p = strstr(reinterpret_cast<char*>(m_Data), c);
    if (p == nullptr) return false;
    return true;
}

String String::Concat(String& a, String& b) noexcept {
    return a + b;
}

String String::Concat(Array<String>& array) const noexcept {

    size_t totalSize = 0;
    for(auto it = array.cbegin(); it != array.cend(); ++it) totalSize += it->GetLength();

    // Push what we already have
    char* ch = new char[GetLength() + totalSize];
    strcpy(ch, reinterpret_cast<char*>(m_Data));

    size_t offset = GetLength();
    for(size_t i = 0; i < array.GetLength(); ++i) {
        auto s = array[i].GetLength();

        for(size_t c = 0; c < s; ++c) {
            ch[c + offset] = array[i][c];
        }

        offset += s;
    }

    String ret = ch;
    delete[] ch;
    return ret;
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

Boolean String::EndsWith(const char *c, StringComparison options) const noexcept {
    const size_t s = strlen(c);
    switch (options) {
        case StringComparison::CaseSensitive:
            if (strncmp(reinterpret_cast<char*>(m_Data) + GetLength() - s, c, s) == 0) return true;
            break;
        case StringComparison::CaseInsensitive:
            if (strnicmp(reinterpret_cast<char*>(m_Data) + GetLength() - s, c, s) == 0) return true;
            break;
    }
    return false;
}

ssize_t String::IndexOf(char c, StringComparison options) const noexcept {
    char temp[2] = {c, '\0'};
    return InternalIndexOf(temp, 0, 0, options);
}

ssize_t String::IndexOf(char c, int startIndex, StringComparison options) const noexcept {
    char temp[2] = {c, '\0'};
    return InternalIndexOf(temp, startIndex, 0, options);
}

ssize_t String::IndexOf(char c, int startIndex, int count, StringComparison options) const noexcept {
    char temp[2] = {c, '\0'};
    return InternalIndexOf(temp, startIndex, count, options);
}

ssize_t String::IndexOf(const char *c, StringComparison options) const noexcept {
    return InternalIndexOf(c, 0, 0, options);
}

ssize_t String::IndexOf(const char *c, int startIndex, StringComparison options) const noexcept {
    return InternalIndexOf(c, startIndex, 0, options);
}

ssize_t String::IndexOf(const char *c, int startIndex, int count, StringComparison options) const noexcept {
    return InternalIndexOf(c, startIndex, count, options);
}

ssize_t String::IndexOfAny(const char array[]) const noexcept {
    return InternalIndexOfAny(array, 0, GetLength());
}

ssize_t String::IndexOfAny(const char array[], int startIndex) const noexcept {
    return InternalIndexOfAny(array, startIndex, 0);
}

ssize_t String::IndexOfAny(const char array[], int startIndex, int count) const noexcept {
    return InternalIndexOfAny(array, startIndex, count);
}

Array<size_t> String::IndicesOf(const char c) const noexcept {
    if (GetLength() == 0) return Array<size_t>{};

    Queue<size_t> q;
    for (size_t i = 0;; ++i) {
        auto a = (strchr((char *) m_Data + i, c));
        if (a != nullptr) {
            auto idx = a - reinterpret_cast<char*>(m_Data);
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
    for (auto p = strstr(reinterpret_cast<char*>(m_Data), c); p != nullptr; p = strstr(p + inc, c))
        q.Enqueue(p - reinterpret_cast<char*>(m_Data));

    return q.ToArray();
}

String String::Insert(size_t index, char c) const {
    char temp[2] = { c, '\0' };
    return Insert(index, temp);
}

String String::Insert(size_t index, const char* c) const {
    if(index > GetLength()) throw std::out_of_range("index");

    size_t length = strlen(c);
    char* ch = new char[GetLength() + length];

    if(index == 0){
        strcpy(ch, c);
        strcpy(ch + length, reinterpret_cast<char*>(m_Data));
    } else if (index == GetLength()) {
        strcpy(ch, reinterpret_cast<char*>(m_Data));
        strcpy(ch + index, c);
    } else {
        strncpy(ch, reinterpret_cast<char*>(m_Data), index);
        strcpy(ch + index, c);
        strcpy(ch + index + length, reinterpret_cast<char*>(m_Data) + index);
    }

    String ret = ch;
    delete[] ch;
    return ret;
}

ssize_t String::InternalIndexOf(const char *c, int startIndex, int count, StringComparison options) const noexcept {
    if (startIndex > LastIndex()) throw std::out_of_range("startIndex");
    if (GetLength() == 0) return -1;

    char *ret;
    if (count == 0) {
        ret = options == StringComparison::CaseSensitive ? strstr(reinterpret_cast<char*>(m_Data) + startIndex, c) : strcasestr(reinterpret_cast<char*>(m_Data) + startIndex, c);
    } else {
        ret = options == StringComparison::CaseSensitive ? strnstr(reinterpret_cast<char*>(m_Data) + startIndex, c, count) : strnstri(reinterpret_cast<char*>(m_Data) + startIndex, c, count);

    }

    if (ret == nullptr) return -1;
    else return ret - reinterpret_cast<char*>(m_Data);
}

ssize_t String::InternalIndexOfAny(const char array[], int startIndex, int count) const noexcept {
    if (startIndex > LastIndex()) throw std::out_of_range("startIndex");
    size_t arrLength = strlen(array);
    if (arrLength == 0 || GetLength() == 0) return -1;

    ssize_t ret = -1;
    for (size_t i = 0; i < arrLength; ++i) {
        char *p;
        if (count == 0) p = strchr(reinterpret_cast<char*>(m_Data) + startIndex, *(array + i));
        else p = (char *) memchr(m_Data + startIndex, *(array + i), count);

        size_t newFound = p - reinterpret_cast<char*>(m_Data);
        if (p != nullptr && newFound < ret) ret = newFound;
    }

    return ret;
}

ssize_t String::InternalLastIndexOf(const char *c, int startIndex, int count, StringComparison options) const noexcept {
    if (startIndex > LastIndex()) throw std::out_of_range("startIndex");
    if (GetLength() == 0) return -1;

    // To avoid wrong pointer operation
    if (count > GetLength()) {
        count = GetLength() - startIndex;
    }

    size_t end = GetLength() - startIndex;
    size_t start = count > end ? 0 : end - count;

    if (start == 0 && end == GetLength()) {
        auto ch = options == StringComparison::CaseSensitive ? strrstr(reinterpret_cast<char*>(m_Data), c) : strrstri(reinterpret_cast<char*>(m_Data), c);
        if (ch != nullptr) return ch - reinterpret_cast<char*>(m_Data);
    } else {
        char *ch = new char[end - start];
        for (size_t i = 0, j = start; j < end; ++i, ++j) {
            ch[i] = m_Data[j];
        }

        size_t ret = 0;
        auto pos = options == StringComparison::CaseSensitive ? strrstr(ch, c) : strrstri(reinterpret_cast<char*>(m_Data), c);
        if (pos != nullptr) {
            ret = pos - ch + (GetLength() - startIndex - count);
            delete[] ch;
            return ret;
        }
        delete[] ch;
    }

    return -1;
}

ssize_t String::InternalLastIndexOfAny(const char array[], int startIndex, int count) const noexcept {
    if (startIndex > LastIndex()) throw std::out_of_range("startIndex");
    size_t arrayLength = strlen(array);
    if (GetLength() == 0 || arrayLength == 0) return -1;

    // To avoid wrong pointer operation
    if (count > GetLength()) {
        count = GetLength() - startIndex;
    }

    size_t end = GetLength() - startIndex;
    size_t start = count > end ? 0 : end - count;

    if (start == 0 && end == GetLength()) {
        for (size_t i = 0; i < arrayLength; ++i) {
            auto ch = strrchr(reinterpret_cast<char*>(m_Data), *(array + i));
            if (ch != nullptr) return ch - reinterpret_cast<char*>(m_Data);
        }
    } else {
        char *ch = new char[end - start];
        for (size_t i = 0, j = start; j < end; ++i, ++j) {
            ch[i] = m_Data[j];
        }

        size_t ret = 0;

        for (size_t i = 0; i < arrayLength; ++i) {
            auto pos = strrchr(ch, *(array + i));
            if (pos != nullptr) {
                ret = pos - ch + (GetLength() - startIndex - count);
                delete[] ch;
                return ret;
            }
        }

        delete[] ch;
    }

    return -1;
}

Array<String>
String::InternalSplit(const char *c, int count, Array<size_t> &array, StringSplitOptions options) const noexcept {

    if (count > array.GetLength()) count = array.GetLength();

    size_t length = strlen(c);

    //Handle the special case of no replaces and special count.
    if (array.GetLength() == 0) {
        return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    Queue<String> q;

    size_t start = 0;
    size_t end;
    size_t charSize;

    switch (options) {
        case StringSplitOptions::RemoveEmptyEntries:
            if (array[0] != 0) {
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

String String::Join(char separator, Array<String> &arrayString) noexcept {
    char temp[2] = {separator, '\0'};
    return String::Join(temp, arrayString);
}

String String::Join(char separator, Array<String> &&arrayString) noexcept {
    char temp[2] = {separator, '\0'};
    return String::Join(temp, arrayString);
}

String String::Join(const char *separator, Array<String> &arrayString) noexcept {

    if (arrayString.GetLength() == 0) return String{};

    size_t totalSize = 0;
    for (const auto &x: arrayString)
        totalSize += x.GetLength();

    totalSize += arrayString.GetLength();

    char *buffer = new char[totalSize];
    size_t start = 0;
    size_t end = 0;
    size_t length = strlen(separator);

    for (size_t i = 0; i < arrayString.GetLength(); ++i) {
        String stg = arrayString[i];
        start = i == 0 ? 0 : end + length;
        end = i == 0 ? stg.GetLength() : start + stg.GetLength();

        std::copy(stg.GetPointer(), stg.GetPointer() + stg.GetLength(), buffer + start);

        if (i == arrayString.GetLength() - 1)
            *(buffer + end) = '\0';
        else
            std::copy(separator, separator + length, buffer + end);
    }

    String cat = buffer;
    delete[] buffer;

    return cat;
}

String String::Join(const char *separator, Array<String> &&arrayString) noexcept {
    return String::Join(separator, arrayString);
}

ssize_t String::LastIndexOf(char c, StringComparison options) const noexcept {
    char temp[2] = {c, '\0'};
    return InternalLastIndexOf(temp, 0, GetLength(), options);
}

ssize_t String::LastIndexOf(char c, int startIndex, StringComparison options) const noexcept {
    char temp[2] = {c, '\0'};
    return InternalLastIndexOf(temp, startIndex, GetLength(), options);
}

ssize_t String::LastIndexOf(char c, int startIndex, int count, StringComparison options) const noexcept {
    char temp[2] = {c, '\0'};
    return InternalLastIndexOf(temp, startIndex, count, options);
}

ssize_t String::LastIndexOf(const char *c, StringComparison options) const noexcept {
    return InternalLastIndexOf(c, 0, GetLength(), options);
}

ssize_t String::LastIndexOf(const char *c, int startIndex, StringComparison options) const noexcept {
    return InternalLastIndexOf(c, startIndex, GetLength(), options);
}

ssize_t String::LastIndexOf(const char *c, int startIndex, int count, StringComparison options) const noexcept {
    return InternalLastIndexOf(c, startIndex, count, options);
}

ssize_t String::LastIndexOfAny(const char array[]) const noexcept {
    return InternalLastIndexOfAny(array, 0, GetLength());
}

ssize_t String::LastIndexOfAny(const char array[], int startIndex) const noexcept {
    return InternalLastIndexOfAny(array, startIndex, GetLength());
}

ssize_t String::LastIndexOfAny(const char array[], int startIndex, int count) const noexcept {
    return InternalLastIndexOfAny(array, startIndex, count);
}

Array<String> String::Split(char delimiter, int count) {
    if (count < 0) throw std::invalid_argument("count");

    if ((GetLength() == 0)) {
        return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    auto array = IndicesOf(delimiter);
    char temp[2] = {delimiter, '\0'};
    return InternalSplit(temp, count, array);
}

String String::PadLeft(size_t width) const noexcept {
    return PadLeft(width, ' ');
}

String String::PadLeft(size_t width, char padding) const noexcept {

    // Avoids the user to push \0 at the beginning of the string
    if (padding == '\0') padding = '0';

    if (width <= GetLength()) return reinterpret_cast<char*>(m_Data);
    char *c = new char[width];
    for (size_t i = 0; i < width - GetLength(); ++i) c[i] = padding;
    for (size_t i = 0; i < GetLength(); ++i) c[i + width - GetLength()] = m_Data[i];
    String ret = c;
    delete[] c;
    return ret;
}

String String::PadRight(size_t width) const noexcept {
    return PadRight(width, ' ');
}

String String::PadRight(size_t width, char padding) const noexcept {
    // Avoids the user to push \0 at the end of the string
    if (padding == '\0') padding = '0';

    if (width <= GetLength()) return reinterpret_cast<char*>(m_Data);
    char *c = new char[width];
    for (size_t i = 0; i < GetLength(); ++i) c[i] = m_Data[i];
    for (size_t i = 0; i < width - GetLength(); ++i) c[i + GetLength()] = padding;
    String ret = c;
    delete[] c;
    return ret;
}

String String::Remove(int startIndex) const {
    return Remove(startIndex, startIndex);
}

String String::Remove(int startIndex, int count) const {
    if(startIndex < 0) throw std::invalid_argument("startIndex");
    if(count < 0) throw std::invalid_argument("count");
    if(count == 0) return reinterpret_cast<char*>(m_Data);
    if(count > GetLength() - startIndex) count = GetLength() - startIndex;

    if(count == GetLength() && startIndex == 0) return "";

    size_t newSize = GetLength() - count;
    char* ch = new char[newSize];
    for(size_t i = 0; i < startIndex; ++i)
        ch[i] = m_Data[i];

    for(size_t i = startIndex + count, j = startIndex; j < newSize; ++i, ++j)
        ch[j] = m_Data[i];

    ch[newSize] = '\0';

    String ret = ch;
    delete[] ch;
    return ret;
}

String String::Replace(char oldValue, char newValue) const noexcept {
    String s(*this);

    for (size_t i = 0; i < GetLength(); ++i) {
        if (m_Data[i] == oldValue) s[i] = newValue;
        else s[i] = m_Data[i];
    }

    return s;
}

String String::Replace(const char *oldValue, const char *newValue) const noexcept {
    const size_t oldSize = strlen(oldValue);
    const size_t newSize = strlen(newValue);

    // diff < 0 = OLD IS LARGER
    // diff > 0 = NEW IS LARGER
    const int diff = newSize - oldSize;

    if (oldSize == 0) return reinterpret_cast<char*>(m_Data);

    Array<size_t> idx = IndicesOf(oldValue);
    if (idx.GetLength() == 0) return reinterpret_cast<char*>(m_Data);

    size_t newcpSize = GetLength() + diff * (idx.GetLength());
    char *ch = new char[newcpSize];

    for(size_t i = 0, j = 0, l = 0; i < newcpSize;){
        if(j < idx.GetLength() && idx[j] == i - (diff * (int)j))
        {
            for(size_t k = 0; k < newSize; ++i, ++k) {
                ch[i] = newValue[k];
            }
            l += oldSize;
            ++j;
        }
        else {
            ch[i] = m_Data[l];
            ++i;
            ++l;
        }
    }

    ch[newcpSize] = '\0';
    String ret = ch;
    delete[] ch;
    return ret;
}

Array<String> String::Split(const char *delimiter, int count) {
    if (count < 0) throw std::invalid_argument("count");

    if ((GetLength() == 0)) {
        return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(delimiter, count, array);
}

Array<String> String::Split(char delimiter, StringSplitOptions options) const noexcept {
    if (GetLength() == 0) {
        if (options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    auto array = IndicesOf(delimiter);
    char temp[2] = {delimiter, '\0'};
    return InternalSplit(temp, array.GetLength(), array, options);
}

Array<String> String::Split(const char *delimiter, StringSplitOptions options) const noexcept {
    if (GetLength() == 0) {
        if (options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(delimiter, array.GetLength(), array, options);
}

Array<String> String::Split(char delimiter, int count, StringSplitOptions options) const {
    if (count < 0) throw std::invalid_argument("count");

    if (GetLength() == 0) {
        if (options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    auto array = IndicesOf(delimiter);
    char temp[2] = {delimiter, '\0'};
    return InternalSplit(temp, count, array, options);
}

Array<String> String::Split(const char *delimiter, int count, StringSplitOptions options) const {
    if (count < 0) throw std::invalid_argument("count");

    if (GetLength() == 0) {
        if (options == StringSplitOptions::RemoveEmptyEntries) return Array<String>{};
        else return Array<String>{reinterpret_cast<char*>(m_Data)};
    }

    auto array = IndicesOf(delimiter);
    return InternalSplit(delimiter, count, array, options);
}

Boolean String::StartsWith(const char *c, StringComparison options) const noexcept {
    const size_t s = strlen(c);
    switch (options) {
        case StringComparison::CaseSensitive:
            if (strncmp(reinterpret_cast<char*>(m_Data), c, s) == 0) return true;
            break;
        case StringComparison::CaseInsensitive:
            if (strnicmp(reinterpret_cast<char*>(m_Data), c, s) == 0) return true;
            break;
    }
    return false;
}

String String::Substring(int startIndex) const {
    return Substring(startIndex, GetLength() - startIndex);
}

String String::Substring(int startIndex, int length) const {
    if (startIndex < 0) throw std::out_of_range("startIndex");
    if (startIndex > GetLength()) throw std::out_of_range("startIndex");
    if (length < 0) throw std::out_of_range("length");
    if (startIndex > GetLength() - length) throw std::out_of_range("length");

    if (length == 0) return String{};
    if (startIndex == 0 && length == GetLength()) return String{reinterpret_cast<char*>(m_Data)};

    size_t size = length - startIndex;
    char *c = new char[size];
    strncpy(c, reinterpret_cast<char*>(m_Data) + startIndex, length);
    String ret = c;
    delete[] c;
    return ret;
}

Array<Char> String::ToCharArray() const noexcept {
    if (GetLength() == 0) return Array<Char>{};

    Array<Char> array(GetLength());
    for (size_t i = 0; i < array.GetLength(); ++i) {
        array[i] = m_Data[i];
    }

    return array;
}

String String::ToLower() const noexcept{
    char* ch = new char[GetLength()];

    for(size_t i = 0; i < GetLength(); ++i) {
        ch[i] = tolower((unsigned char)m_Data[i]);
    }

    String ret = ch;
    delete[] ch;
    return ret;
}

String String::ToUpper() const noexcept{
    char* ch = new char[GetLength()];

    for(size_t i = 0; i < GetLength(); ++i) {
        ch[i] = toupper((unsigned char)m_Data[i]);
    }

    String ret = ch;
    delete[] ch;
    return ret;
}

String String::Trim() const noexcept {
    return Trim(Space);
}

String String::Trim(char c) const noexcept {
    char temp[2] = {c, '\0'};
    return Trim(temp);
}

String String::Trim(const char *c) const noexcept {
    if (GetLength() == 0) return "";
    if (strcmp(c, "") == 0) return reinterpret_cast<char*>(m_Data);

    auto p = m_Data;
    size_t start = 0;
    const size_t length = strlen(c);
    size_t start2 = 0;
    while (*p == *(c + start2)) {
        ++start;
        ++p;
        ++start2;
        if (start2 == length) start2 = 0;
    }

    size_t end = GetLength() - 1;;
    p = m_Data + end;
    ssize_t end2 = length - 1;
    while (*p == *(c + end2)) {
        --end;
        --p;
        --end2;
        if (end2 == -1) end2 = length - 1;
    }

    end = (size_t) (m_Data + end) - (size_t) m_Data;
    char *temp = new char[end - start + 1];
    std::copy(m_Data + start, m_Data + end + 1, temp);
    temp[end - start + 1] = '\0';
    String ret = temp;
    delete[] temp;
    return ret;
}

String String::TrimEnd() const noexcept {
    return TrimEnd(Space);
}

String String::TrimEnd(char c) const noexcept {
    char temp[2] = {c, '\0'};
    return Trim(temp);
}

String String::TrimEnd(const char *c) const noexcept {
    if (GetLength() == 0) return "";
    if (strcmp(c, "") == 0) return reinterpret_cast<char*>(m_Data);

    size_t end = GetLength() - 1;;
    auto p = m_Data + end;
    const size_t length = strlen(c);
    ssize_t start2 = length - 1;
    while (*p == *(c + start2)) {
        --end;
        --p;
        --start2;
        if (start2 == -1) start2 = length - 1;
    }

    end = (size_t) (m_Data + end) - (size_t) m_Data;
    char *temp = new char[end + 1];
    std::copy(m_Data, m_Data + end + 1, temp);
    temp[end + 1] = '\0';
    String ret = temp;
    delete[] temp;
    return ret;
}

String String::TrimStart() const noexcept {
    return TrimStart(Space);
}

String String::TrimStart(char c) const noexcept {
    char temp[2] = {c, '\0'};
    return TrimStart(temp);
}

String String::TrimStart(const char *c) const noexcept {
    if (GetLength() == 0) return "";
    if (strcmp(c, "") == 0) return reinterpret_cast<char*>(m_Data);

    auto p = m_Data;
    size_t start = 0;
    const size_t length = strlen(c);
    size_t start2 = 0;
    while (*p == *(c + start2)) {
        ++start;
        ++p;
        ++start2;
        if (start2 == length) start2 = 0;
    }

    return reinterpret_cast<char*>(m_Data) + start;
}
