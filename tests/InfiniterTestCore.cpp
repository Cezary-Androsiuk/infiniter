#include <gtest/gtest.h>
#include <utility>
#include "InfiniterCore.hpp"
#include "InfiniterException.hpp"

class InfiniterTestCore : public InfiniterCore<InfiniterTestCore> {
public:
    using Base = InfiniterCore<InfiniterTestCore>;

    InfiniterTestCore() noexcept : Base() {}
    InfiniterTestCore(int64_t p_value) : Base(p_value) {}
    InfiniterTestCore(int64_t p_value, isize_t p_capacity) : Base(p_value, p_capacity) {}
    InfiniterTestCore(icell_t p_value, isize_t p_capacity, bool p_negative_value) : Base(p_value, p_capacity, p_negative_value) {}
    InfiniterTestCore(const icell_t *p_array, isize_t p_size, bool p_negative_value = false) : Base(p_array, p_size, p_negative_value) {}
    InfiniterTestCore(const InfiniterTestCore &p_source) : Base(p_source) {}
    InfiniterTestCore(InfiniterTestCore &&p_source) noexcept : Base(std::move(p_source)) {}

    InfiniterTestCore& operator=(const InfiniterTestCore& other) {
        Base::operator=(other);
        return *this;
    }
    InfiniterTestCore& operator=(InfiniterTestCore&& other) noexcept {
        Base::operator=(std::move(other));
        return *this;
    }
    using Base::operator=;

    using Base::clear;
    using Base::clearReserved;
    using Base::equalMagnitude;
    using Base::differsMagnitude;
    using Base::greaterMagnitude;
    using Base::smallerMagnitude;
    using Base::greaterEqualMagnitude;
    using Base::smallerEqualMagnitude;
};
// ---------------------------------------------------------
// Constructor & Initialization Tests
// ---------------------------------------------------------

TEST(InfiniterCoreTest, DefaultConstructor) {
    InfiniterTestCore ic;
    EXPECT_EQ(ic.getSize(), 1);
    EXPECT_FALSE(ic.getSign());
    EXPECT_EQ(ic.getData()[0], 0);
}

TEST(InfiniterCoreTest, ValueConstructors) {
    InfiniterTestCore ic1(123LL);
    EXPECT_EQ(ic1.getData()[0], 123);
    EXPECT_FALSE(ic1.getSign());

    InfiniterTestCore ic2(-123LL);
    EXPECT_EQ(ic2.getData()[0], 123);
    EXPECT_TRUE(ic2.getSign());

    InfiniterTestCore ic3(-456LL, 10);
    EXPECT_EQ(ic3.getData()[0], 456);
    EXPECT_TRUE(ic3.getSign());
    EXPECT_GE(ic3.getCapacity(), 10);
}

TEST(InfiniterCoreTest, ArrayConstructor) {
    icell_t arr[] = { 10, 20, 0 };
    InfiniterTestCore ic(arr, 3, true);

    EXPECT_EQ(ic.getSize(), 2);
    EXPECT_TRUE(ic.getSign());
    EXPECT_EQ(ic.getData()[1], 20);

    EXPECT_THROW(InfiniterTestCore(arr, 0), InfiniterException::InvalidSize);
}

// ---------------------------------------------------------
// Size, Trimming, and Normalization Tests
// ---------------------------------------------------------

TEST(InfiniterCoreTest, GetRealSize) {
    icell_t arr[] = { 1, 0, 0, 0 };
    InfiniterTestCore ic(arr, 4);

    ic.setSizeWithExtend(4);
    EXPECT_EQ(ic.getSize(), 4);
    EXPECT_EQ(ic.getRealSize(), 1);
}

TEST(InfiniterCoreTest, SetSizeWithExtend) {
    InfiniterTestCore ic;
    ic.setSizeWithExtend(150);

    EXPECT_EQ(ic.getSize(), 150);
    EXPECT_GE(ic.getCapacity(), 150);
}

TEST(InfiniterCoreTest, NormalizeCorrectsZeroSign) {
    InfiniterTestCore ic;
    ic.getData()[0] = 0;
    ic.setSign(true);

    ic.normalize();
    EXPECT_FALSE(ic.getSign());
}

// ---------------------------------------------------------
// Assign Operators Tests
// ---------------------------------------------------------

TEST(InfiniterCoreTest, AssignScalar) {
    InfiniterTestCore ic;
    ic.assign(999ULL, true);

    EXPECT_EQ(ic.getData()[0], 999);
    EXPECT_TRUE(ic.getSign());
    EXPECT_EQ(ic.getSize(), 1);
}

TEST(InfiniterCoreTest, AssignArray) {
    InfiniterTestCore ic;
    icell_t arr[] = { 5, 10, 15 };
    ic.assign(arr, 3, false);

    EXPECT_EQ(ic.getSize(), 3);
    EXPECT_EQ(ic.getData()[2], 15);
}

TEST(InfiniterCoreTest, OperatorAssignIsvalT) {
    InfiniterTestCore ic;
    ic = -777LL;

    EXPECT_TRUE(ic.getSign());
    EXPECT_EQ(ic.getData()[0], 777);
}

// ---------------------------------------------------------
// State Checkers
// ---------------------------------------------------------

TEST(InfiniterCoreTest, BooleanStates) {
    InfiniterTestCore zero;
    EXPECT_TRUE(zero.is0());
    EXPECT_FALSE(zero.toBool());
    EXPECT_FALSE(static_cast<bool>(zero));

    InfiniterTestCore one(1LL);
    EXPECT_TRUE(one.is1());
    EXPECT_TRUE(one.isPositive1());
    EXPECT_FALSE(one.isNegative1());
    EXPECT_TRUE(one.toBool());
    EXPECT_TRUE(static_cast<bool>(one));

    InfiniterTestCore minus_two(-2LL);
    EXPECT_TRUE(minus_two.is2());
    EXPECT_TRUE(minus_two.isNegative2());
    EXPECT_FALSE(minus_two.isPositive2());
}

// ---------------------------------------------------------
// Sign & Math Operations
// ---------------------------------------------------------

TEST(InfiniterCoreTest, UnaryMinusOperator) {
    InfiniterTestCore ic(500LL);
    InfiniterTestCore negated = -ic;

    EXPECT_FALSE(ic.getSign());
    EXPECT_TRUE(negated.getSign());
    EXPECT_EQ(negated.getData()[0], 500);

    InfiniterTestCore double_negated = -negated;
    EXPECT_FALSE(double_negated.getSign());
}

TEST(InfiniterCoreTest, AbsoluteValue) {
    InfiniterTestCore ic(-80LL);
    InfiniterTestCore abs_ic = InfiniterTestCore::abs(ic);

    EXPECT_FALSE(abs_ic.getSign());
    EXPECT_EQ(abs_ic.getData()[0], 80);

    ic.abs();
    EXPECT_FALSE(ic.getSign());
}

// ---------------------------------------------------------
// Magnitude Comparisons
// ---------------------------------------------------------

TEST(InfiniterCoreTest, MagnitudeComparisonsScalar) {
    InfiniterTestCore ic(-50LL);

    EXPECT_TRUE(ic.equalMagnitude(50));
    EXPECT_FALSE(ic.differsMagnitude(50));
    EXPECT_TRUE(ic.greaterMagnitude(20));
    EXPECT_FALSE(ic.smallerMagnitude(20));
    EXPECT_TRUE(ic.greaterEqualMagnitude(50));
    EXPECT_TRUE(ic.smallerEqualMagnitude(50));
}

TEST(InfiniterCoreTest, MagnitudeComparisonsObject) {
    icell_t arr1[] = { 1, 2 };
    icell_t arr2[] = { 1, 3 };

    InfiniterTestCore ic1(arr1, 2, true);
    InfiniterTestCore ic2(arr2, 2, false);

    EXPECT_TRUE(ic2.greaterMagnitude(ic1));
    EXPECT_TRUE(ic1.smallerMagnitude(ic2));
    EXPECT_FALSE(ic1.equalMagnitude(ic2));
}

// ---------------------------------------------------------
// Standard Comparisons (Value & Sign)
// ---------------------------------------------------------

TEST(InfiniterCoreTest, StandardComparisonsScalarWithSign) {
    InfiniterTestCore ic(-50LL);

    EXPECT_TRUE(ic.smaller(20, false));
    EXPECT_TRUE(ic.smaller(40, true));
    EXPECT_TRUE(ic.greater(60, true));
    EXPECT_TRUE(ic.equal(50, true));
    EXPECT_TRUE(ic.differs(50, false));
}

TEST(InfiniterCoreTest, StandardComparisonsIsvalT) {
    InfiniterTestCore ic(-100LL);

    EXPECT_TRUE(ic < 50LL);
    EXPECT_TRUE(ic > -200LL);
    EXPECT_TRUE(ic == -100LL);
    EXPECT_TRUE(ic != 100LL);
    EXPECT_TRUE(ic <= -100LL);
    EXPECT_TRUE(ic >= -150LL);
}

TEST(InfiniterCoreTest, StandardComparisonsObject) {
    InfiniterTestCore ic1(-100LL);
    InfiniterTestCore ic2(50LL);
    InfiniterTestCore ic3(-200LL);
    InfiniterTestCore ic4(-100LL);

    EXPECT_TRUE(ic1 < ic2);
    EXPECT_TRUE(ic2 > ic1);

    EXPECT_TRUE(ic1 > ic3);
    EXPECT_TRUE(ic3 < ic1);

    EXPECT_TRUE(ic1 == ic4);
    EXPECT_TRUE(ic1 != ic2);
    EXPECT_TRUE(ic1 <= ic4);
    EXPECT_TRUE(ic1 >= ic3);
}

// ---------------------------------------------------------
// Access Operators
// ---------------------------------------------------------

TEST(InfiniterCoreTest, ArraySubscriptOperator) {
    icell_t arr[] = { 10, 20 };
    InfiniterTestCore ic(arr, 2);

    EXPECT_EQ(ic[0], 10);
    EXPECT_EQ(ic[1], 20);

    ic[1] = 99;
    EXPECT_EQ(ic.getCell(1), 99);
}

TEST(InfiniterCoreTest, CellAccessThrowsOutOfBounds) {
    InfiniterTestCore ic(100LL);

    EXPECT_NO_THROW(ic.getCell(0));
    EXPECT_THROW(ic.getCell(1), InfiniterException::OutOfRange);
}

// ---------------------------------------------------------
// Operations Beyond SBO_CAPACITY Tests
// ---------------------------------------------------------

TEST(InfiniterCoreTest, ArrayConstructorBeyondSBO) {
    // Requires SBO_CAPACITY to be well-defined in InfiniterShared.hpp
    icell_t arr[] = { 1, 2, 3, 4, 5 };
    InfiniterTestCore ic(arr, 5, true);

    EXPECT_EQ(ic.getSize(), 5);
    EXPECT_GE(ic.getCapacity(), 5);
    EXPECT_GT(ic.getCapacity(), SBO_CAPACITY); // confirms heap allocation
    EXPECT_TRUE(ic.getSign());
    EXPECT_EQ(ic.getData()[4], 5);
}

TEST(InfiniterCoreTest, SetSizeWithExtendBeyondSBO) {
    InfiniterTestCore ic;
    EXPECT_LE(ic.getCapacity(), SBO_CAPACITY);

    isize_t new_size = SBO_CAPACITY + 10;
    ic.setSizeWithExtend(new_size);

    EXPECT_EQ(ic.getSize(), new_size);
    EXPECT_GE(ic.getCapacity(), new_size);
    EXPECT_EQ(ic.getData()[new_size - 1], 0);
}

TEST(InfiniterCoreTest, AssignArrayBeyondSBO) {
    InfiniterTestCore ic;
    icell_t arr[] = { 10, 20, 30, 40, 50, 60 };
    ic.assign(arr, 6, false);

    EXPECT_EQ(ic.getSize(), 6);
    EXPECT_GT(ic.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(ic.getData()[5], 60);
}

TEST(InfiniterCoreTest, CopyAndMoveBeyondSBO) {
    icell_t arr[] = { 1, 2, 3, 4, 5 };
    InfiniterTestCore src(arr, 5);

    InfiniterTestCore copied(src);
    EXPECT_EQ(copied.getSize(), 5);
    EXPECT_GT(copied.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(copied.getData()[4], 5);

    InfiniterTestCore moved(std::move(src));
    EXPECT_EQ(moved.getSize(), 5);
    EXPECT_GT(moved.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(moved.getData()[4], 5);
}

TEST(InfiniterCoreTest, OptimizeShrinksToSBO) {
    icell_t arr[] = { 1, 2, 3, 4, 5 };
    InfiniterTestCore ic(arr, 5);

    EXPECT_GT(ic.getCapacity(), SBO_CAPACITY);

    ic = 99LL; // assigns small value, logically size drops to 1
    EXPECT_EQ(ic.getSize(), 1);
    EXPECT_GT(ic.getCapacity(), SBO_CAPACITY); // capacity remains large before shrink

    ic.optimize(); // forces shrink to SBO

    EXPECT_EQ(ic.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(ic.getData()[0], 99);
}

TEST(InfiniterCoreTest, ComparisonsBeyondSBO) {
    icell_t arr1[] = { 1, 2, 3, 4, 5 };
    icell_t arr2[] = { 1, 2, 3, 4, 6 };
    icell_t arr3[] = { 1, 2, 3, 4, 5, 0, 0 }; // Trailing zeros should normalize out

    InfiniterTestCore ic1(arr1, 5);
    InfiniterTestCore ic2(arr2, 5);
    InfiniterTestCore ic3(arr3, 7); // Will normalize to size 5

    EXPECT_TRUE(ic1 < ic2);
    EXPECT_TRUE(ic2 > ic1);
    EXPECT_TRUE(ic1 == ic3);
    EXPECT_FALSE(ic1 == ic2);
}

/// complex tests

// ---------------------------------------------------------
// Complex Flow Tests
// ---------------------------------------------------------

TEST(InfiniterCoreComplexTest, SboToHeapAndBackToSbo) {
    // Start with SBO
    InfiniterTestCore ic(100LL);
    EXPECT_EQ(ic.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(ic.getSize(), 1);

    // Expand to heap
    isize_t large_size = SBO_CAPACITY + 10;
    ic.setSizeWithExtend(large_size);
    EXPECT_GE(ic.getCapacity(), large_size);
    EXPECT_EQ(ic.getSize(), large_size);

    // Fill top with non-zero to verify it works
    ic[large_size - 1] = 0xDEADBEEF;
    EXPECT_EQ(ic.getCell(large_size - 1), 0xDEADBEEF);

    // Truncate size logically
    ic.setSize(1);
    EXPECT_EQ(ic.getSize(), 1);
    EXPECT_GE(ic.getCapacity(), large_size); // Capacity should still be large

    // Force memory shrink to fit logical size (which is 1, so it should trigger SBO fallback)
    ic.optimize();

    EXPECT_EQ(ic.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(ic.getSize(), 1);
    EXPECT_EQ(ic.getData()[0], 100) << "Original data in cell 0 should survive the roundtrip";
}

TEST(InfiniterCoreComplexTest, MemoryZeroingAfterExtendShrinkExtend) {
    InfiniterTestCore ic;

    // Extend and put some garbage data
    ic.setSizeWithExtend(5);
    ic[1] = 11;
    ic[2] = 22;
    ic[3] = 33;
    ic[4] = 44;

    // Shrink logical size (memory stays allocated)
    ic.setSize(2);
    EXPECT_EQ(ic.getSize(), 2);

    // Re-extend size. The cells previously containing 22, 33, 44 must be zeroed
    ic.setSize(5);

    EXPECT_EQ(ic[0], 0);
    EXPECT_EQ(ic[1], 11);
    EXPECT_EQ(ic[2], 0) << "Cell should be zeroed after re-expanding size";
    EXPECT_EQ(ic[3], 0);
    EXPECT_EQ(ic[4], 0);
}

TEST(InfiniterCoreComplexTest, ArrayNormalizationThenManualExpansion) {
    // Input array has multiple leading zeros
    icell_t arr[] = { 55, 66, 0, 0, 0 };
    InfiniterTestCore ic(arr, 5, true);

    // Constructor should normalize it down to size 2
    EXPECT_EQ(ic.getSize(), 2);
    EXPECT_GE(ic.getCapacity(), 5); // Kept the requested capacity
    EXPECT_TRUE(ic.getSign());

    // Manually force size back up without extending capacity
    ic.setSize(4);
    EXPECT_EQ(ic.getSize(), 4);
    EXPECT_EQ(ic[3], 0);

    // Trim again manually
    ic.trim();
    EXPECT_EQ(ic.getSize(), 2);
}

TEST(InfiniterCoreComplexTest, ComplexMoveSemanticsBetweenMemoryStates) {
    // ic_heap is large (heap)
    icell_t arr[] = { 1, 2, 3, 4, 5 };
    InfiniterTestCore ic_heap(arr, 5);

    // ic_sbo is small (stack)
    InfiniterTestCore ic_sbo(99LL);

    // Move heap to sbo
    ic_sbo = std::move(ic_heap);

    // Destination should now use heap
    EXPECT_GT(ic_sbo.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(ic_sbo.getSize(), 5);
    EXPECT_EQ(ic_sbo[4], 5);

#if I_ENSURE_NEW_OBJECT_AFTER_MOVE
    // Source should revert to SBO and be safe to use
    EXPECT_EQ(ic_heap.getCapacity(), SBO_CAPACITY);
    EXPECT_EQ(ic_heap.getSize(), 1); // Reset logic should set size to 1

    ic_heap.setSizeWithExtend(10);
    ic_heap[9] = 123;
    EXPECT_EQ(ic_heap[9], 123) << "Moved-from object must be perfectly usable if I_ENSURE_NEW_OBJECT_AFTER_MOVE is true";
#endif
}

TEST(InfiniterCoreComplexTest, AssignLargeArrayToSmallObjectAndOptimize) {
    InfiniterTestCore ic(1LL); // Starts at SBO
    EXPECT_EQ(ic.getCapacity(), SBO_CAPACITY);

    // Force array assignment that heavily exceeds current capacity
    icell_t large_arr[20];
    for(int i = 0; i < 20; i++) large_arr[i] = i + 1;

    ic.assign(large_arr, 20, false);

    EXPECT_EQ(ic.getSize(), 20);
    EXPECT_GE(ic.getCapacity(), 20);
    EXPECT_EQ(ic[19], 20);

    // Assign a tiny value to the huge object
    ic = 5LL; // Uses operator=(isval_t)

    EXPECT_EQ(ic.getSize(), 1);
    EXPECT_GE(ic.getCapacity(), 20); // Still hogging memory

    // Force shrink
    ic.optimize();

    EXPECT_EQ(ic.getSize(), 1);
    EXPECT_EQ(ic.getCapacity(), SBO_CAPACITY) << "Should shrink back to SBO";
    EXPECT_EQ(ic[0], 5);
}

TEST(InfiniterCoreComplexTest, ExceptionFlowDuringCapacityChanges) {
    InfiniterTestCore ic(123LL);

    ic.setSizeWithExtend(10);
    EXPECT_NO_THROW(ic.getCell(9));

    // Shrink size
    ic.setSize(2);

    // Accessing index 9 should now throw, even if memory is technically still allocated
    EXPECT_THROW(ic.getCell(9), InfiniterException::OutOfRange);

    // Optimizing memory shouldn't affect the logical out-of-range behavior
    ic.optimize();
    EXPECT_THROW(ic.getCell(9), InfiniterException::OutOfRange);

    // Cell 1 should still be completely fine
    EXPECT_NO_THROW(ic.getCell(1));
}
