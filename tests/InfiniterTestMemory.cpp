#include <gtest/gtest.h>
#include "InfiniterMemory.hpp"


class InfiniterTestMemory : public InfiniterMemory {
public:
    InfiniterTestMemory() : InfiniterMemory() {}
    explicit InfiniterTestMemory(isize_t p_capacity) : InfiniterMemory(p_capacity) {}
    InfiniterTestMemory(const InfiniterTestMemory& p_source) : InfiniterMemory(p_source) {}
    InfiniterTestMemory(InfiniterTestMemory&& p_source) noexcept : InfiniterMemory(std::move(p_source)) {}

    /// Getters
    icell_t* getData() const { return m_data; }
    isize_t getCapacity() const { return m_capacity; }
    bool sboActive() const { return isSBOActive(); }

    /// Expose protected methods
    using InfiniterMemory::reset;
    using InfiniterMemory::reserve;
    using InfiniterMemory::extend;
    using InfiniterMemory::shrink;
    using InfiniterMemory::assign;

    InfiniterTestMemory& operator=(const InfiniterTestMemory& other) {
        InfiniterMemory::operator=(other);
        return *this;
    }

    InfiniterTestMemory& operator=(InfiniterTestMemory&& other) noexcept {
        InfiniterMemory::operator=(std::move(other));
        return *this;
    }

    // Helper method to set data for testing
    void fillData(icell_t start_val) {
        for (isize_t i = 0; i < m_capacity; ++i) {
            m_data[i] = start_val + i;
        }
    }
};

// ---------------------------------------------------------
// Constructor Tests
// ---------------------------------------------------------

TEST(InfiniterMemoryTest, DefaultConstructorUsesSBO) {
    // Check if default initialization sets up SBO correctly
    InfiniterTestMemory im;
    EXPECT_TRUE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), SBO_CAPACITY);
    EXPECT_NE(im.getData(), nullptr);

    // Verify memory is cleared if IM_CLEAR_ALLOCATED_MEMORY is true
#if IM_CLEAR_ALLOCATED_MEMORY
    for (isize_t i = 0; i < SBO_CAPACITY; ++i) {
        EXPECT_EQ(im.getData()[i], ICELL_C(0));
    }
#endif
}

TEST(InfiniterMemoryTest, ParameterConstructorSBO) {
    // Capacities <= SBO_CAPACITY should use stack
    InfiniterTestMemory im(SBO_CAPACITY);
    EXPECT_TRUE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), SBO_CAPACITY);
}

TEST(InfiniterMemoryTest, ParameterConstructorHeap) {
    // Capacities > SBO_CAPACITY should allocate on heap
    isize_t cap = SBO_CAPACITY + 10;
    InfiniterTestMemory im(cap);
    EXPECT_FALSE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), cap);
}

// ---------------------------------------------------------
// Copy & Move Constructors
// ---------------------------------------------------------

TEST(InfiniterMemoryTest, CopyConstructorSBO) {
    // Copying SBO to SBO
    InfiniterTestMemory src;
    src.fillData(100);

    InfiniterTestMemory dest(src);
    EXPECT_TRUE(dest.sboActive());
    EXPECT_EQ(dest.getCapacity(), SBO_CAPACITY);
    EXPECT_NE(src.getData(), dest.getData()); // deep copy
    EXPECT_EQ(dest.getData()[0], 100);
}

TEST(InfiniterMemoryTest, CopyConstructorHeap) {
    // Copying Heap to Heap
    InfiniterTestMemory src(SBO_CAPACITY + 5);
    src.fillData(200);

    InfiniterTestMemory dest(src);
    EXPECT_FALSE(dest.sboActive());
    EXPECT_EQ(dest.getCapacity(), SBO_CAPACITY + 5);
    EXPECT_NE(src.getData(), dest.getData());
    EXPECT_EQ(dest.getData()[0], 200);
}

TEST(InfiniterMemoryTest, MoveConstructorSBO) {
    // Moving SBO (effectively copies data, resets source if I_ENSURE_NEW_OBJECT_AFTER_MOVE)
    InfiniterTestMemory src;
    src.fillData(300);

    InfiniterTestMemory dest(std::move(src));
    EXPECT_TRUE(dest.sboActive());
    EXPECT_EQ(dest.getData()[0], 300);

#if I_ENSURE_NEW_OBJECT_AFTER_MOVE
    EXPECT_TRUE(src.sboActive());
    EXPECT_EQ(src.getCapacity(), SBO_CAPACITY);
#endif
}

TEST(InfiniterMemoryTest, MoveConstructorHeap) {
    // Moving Heap transfers pointer
    InfiniterTestMemory src(SBO_CAPACITY + 10);
    src.fillData(400);
    icell_t* original_ptr = src.getData();

    InfiniterTestMemory dest(std::move(src));
    EXPECT_FALSE(dest.sboActive());
    EXPECT_EQ(dest.getData(), original_ptr); // pointer stolen

#if I_ENSURE_NEW_OBJECT_AFTER_MOVE
    EXPECT_TRUE(src.sboActive()); // source reverted to SBO
#endif
}

// ---------------------------------------------------------
// Assignment Operators
// ---------------------------------------------------------

TEST(InfiniterMemoryTest, CopyAssignment) {
    InfiniterTestMemory src(SBO_CAPACITY + 5);
    src.fillData(500);

    InfiniterTestMemory dest; // Currently SBO
    dest = src; // Now should be heap

    EXPECT_FALSE(dest.sboActive());
    EXPECT_EQ(dest.getCapacity(), SBO_CAPACITY + 5);
    EXPECT_EQ(dest.getData()[0], 500);
}

TEST(InfiniterMemoryTest, MoveAssignment) {
    InfiniterTestMemory src(SBO_CAPACITY + 5);
    src.fillData(600);
    icell_t* original_ptr = src.getData();

    InfiniterTestMemory dest;
    dest = std::move(src);

    EXPECT_FALSE(dest.sboActive());
    EXPECT_EQ(dest.getData(), original_ptr);
}

TEST(InfiniterMemoryTest, SelfAssignment) {
    // Should not crash or invalidate memory
    InfiniterTestMemory im(SBO_CAPACITY + 2);
    im.fillData(700);
    icell_t* ptr = im.getData();

    im = im;

    EXPECT_EQ(im.getData(), ptr);
    EXPECT_EQ(im.getData()[0], 700);
}

// ---------------------------------------------------------
// Reserve, Extend, Reset
// ---------------------------------------------------------

TEST(InfiniterMemoryTest, ReserveLarger) {
    // Transition from SBO to Heap
    InfiniterTestMemory im;
    im.fillData(800);

    isize_t new_cap = SBO_CAPACITY + 10;
    im.reserve(new_cap);

    EXPECT_FALSE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), new_cap);
    EXPECT_EQ(im.getData()[0], 800); // Old data retained
    EXPECT_EQ(im.getData()[new_cap - 1], 0); // New data zeroed
}

TEST(InfiniterMemoryTest, ReserveSmallerDoesNothing) {
    InfiniterTestMemory im(SBO_CAPACITY + 5);
    im.fillData(900);
    icell_t* old_ptr = im.getData();

    im.reserve(SBO_CAPACITY); // Should hit UNLIKELY return

    EXPECT_EQ(im.getCapacity(), SBO_CAPACITY + 5);
    EXPECT_EQ(im.getData(), old_ptr);
}

TEST(InfiniterMemoryTest, ExtendCapacity) {
    InfiniterTestMemory im(SBO_CAPACITY + 2);
    isize_t initial_cap = im.getCapacity();

    im.extend(5);
    EXPECT_EQ(im.getCapacity(), initial_cap + 5);
    EXPECT_FALSE(im.sboActive());
}

TEST(InfiniterMemoryTest, ResetRestoresSBO) {
    InfiniterTestMemory im(SBO_CAPACITY + 10);
    EXPECT_FALSE(im.sboActive());

    im.reset();

    EXPECT_TRUE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(im.getData()[0], 0);
}

// ---------------------------------------------------------
// Shrink Tests (Including Bug Exposing Test)
// ---------------------------------------------------------

TEST(InfiniterMemoryTest, ShrinkHeapToHeap) {
    InfiniterTestMemory im(SBO_CAPACITY + 10);
    im.fillData(1000);

    isize_t target = SBO_CAPACITY + 2;
    im.shrink(target);

    EXPECT_FALSE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), target);
    EXPECT_EQ(im.getData()[0], 1000);
}

TEST(InfiniterMemoryTest, ShrinkHeapToSBO) {
    InfiniterTestMemory im(SBO_CAPACITY + 5);
    im.fillData(2000);

    im.shrink(SBO_CAPACITY);

    EXPECT_TRUE(im.sboActive());
    EXPECT_EQ(im.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(im.getData()[0], 2000);
}

TEST(InfiniterMemoryTest, ShrinkLargerDoesNothing) {
    InfiniterTestMemory im(SBO_CAPACITY + 5);
    icell_t* old_ptr = im.getData();

    im.shrink(SBO_CAPACITY + 10);

    EXPECT_EQ(im.getCapacity(), SBO_CAPACITY + 5);
    EXPECT_EQ(im.getData(), old_ptr);
}

// This test will likely fail with your current implementation,
// proving the flaw in shrink logic when p_target_capacity < SBO_CAPACITY.
TEST(InfiniterMemoryTest, ShrinkBelowSBO_BugTest) {
    InfiniterTestMemory im(SBO_CAPACITY + 5);

    // Attempting to shrink below SBO size
    isize_t target = SBO_CAPACITY - 1;
    if (target < 1) target = 1; // Safeguard if SBO is 1

    im.shrink(target);

    // It should fallback to SBO size and activate SBO.
    // Your code currently sets sboActive to false and creates a small heap.
    EXPECT_TRUE(im.sboActive()) << "BUG DETECTED: Object did not revert to SBO when shrink target was less than SBO_CAPACITY.";
    EXPECT_GE(im.getCapacity(), SBO_CAPACITY) << "BUG DETECTED: Capacity dropped below absolute minimum SBO_CAPACITY.";
}
