#include <gtest/gtest.h>
#include "InfiniterMemory.hpp"

// =========================================================================
// Wraper testowy: wyciąga chronione metody do sekcji publicznej,
// abyśmy mogli je swobodnie wołać w testach.
// =========================================================================
class TestableInfiniterMemory : public InfiniterMemory {
public:
    // Jawne, publiczne konstruktory wołające chronione konstruktory bazy
    TestableInfiniterMemory() noexcept : InfiniterMemory() {}
    explicit TestableInfiniterMemory(isize_t p_capacity) : InfiniterMemory(p_capacity) {}

    // Metody - tutaj "using" działa poprawnie i zmienia dostęp na public
    using InfiniterMemory::reset;
    using InfiniterMemory::reserve;
    using InfiniterMemory::extend;
    using InfiniterMemory::shrink;
    using InfiniterMemory::assign;
    using InfiniterMemory::isSBOActive;

    // Pola
    using InfiniterMemory::m_capacity;
    using InfiniterMemory::m_data;
};

// =========================================================================
// Właściwe testy
// =========================================================================

// Testujemy konstruktor domyślny
TEST(InfiniterMemoryTests, DefaultConstructorInitializesCorrectly) {
    TestableInfiniterMemory mem;

    // Sprawdzamy stan początkowy - SBO powinno być aktywne
    EXPECT_TRUE(mem.isSBOActive());
    // Jeśli zakładasz, że domyślny konstruktor ustawia capacity na 0 lub SBO_CAPACITY:
    EXPECT_EQ(mem.m_capacity, SBO_CAPACITY); // Zmień na SBO_CAPACITY jeśli taka jest Twoja logika
}

// Testujemy rezerwację pamięci w ramach Small Buffer Optimization (SBO)
TEST(InfiniterMemoryTests, ReserveWithinSBO) {
    TestableInfiniterMemory mem;

    // Rezerwacja małej ilości danych, która powinna zmieścić się w m_sbo_buffer
    mem.reserve(SBO_CAPACITY);

    EXPECT_TRUE(mem.isSBOActive());
    EXPECT_GE(mem.m_capacity, SBO_CAPACITY);
}

// Testujemy alokację sterty (Heap), gdy przekraczamy pojemność SBO
TEST(InfiniterMemoryTests, ReserveExceedsSBOAllocatesHeap) {
    TestableInfiniterMemory mem;
    isize_t large_capacity = SBO_CAPACITY + 10;

    mem.reserve(large_capacity);

    // SBO powinno przestać być aktywne
    EXPECT_FALSE(mem.isSBOActive());
    EXPECT_GE(mem.m_capacity, large_capacity);
    EXPECT_NE(mem.m_data, nullptr); // Wskaźnik m_data nie może być nullem
}

// Testujemy logikę rozszerzania pamięci
TEST(InfiniterMemoryTests, ExtendIncreasesCapacity) {
    TestableInfiniterMemory mem(10); // Użycie konstruktora z pojemnością
    isize_t initial_capacity = mem.m_capacity;

    mem.extend(5); // Dodajemy 5

    EXPECT_GE(mem.m_capacity, initial_capacity + 5);
}

// Testujemy Reset
TEST(InfiniterMemoryTests, ResetClearsDataAndRestoresSBO) {
    TestableInfiniterMemory mem(100); // Wymuszamy alokację na stercie
    EXPECT_FALSE(mem.isSBOActive());

    mem.reset();

    EXPECT_TRUE(mem.isSBOActive());
    // Oczekujemy, że reset wyzeruje capacity lub ustawi na początkowe
    EXPECT_EQ(mem.m_capacity, SBO_CAPACITY);
}
