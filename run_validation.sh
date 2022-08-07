#!/usr/bin/env sh

rm -rf *.result
TEST_NAME=CONCAT_PASS make validate
TEST_NAME=CONCAT_FAIL make validate
TEST_NAME=ROSE_PASS make validate
TEST_NAME=ROSE_FAIL make validate
TEST_NAME=FELL_PASS make validate
TEST_NAME=FELL_FAIL make validate
TEST_NAME=PAST_PASS make validate
TEST_NAME=PAST_FAIL make validate
TEST_NAME=STABLE_PASS make validate
TEST_NAME=STABLE_FAIL make validate
TEST_NAME=INTERSECT_PASS make validate
TEST_NAME=INTERSECT_FAIL make validate
TEST_NAME=THROUGHOUT_PASS make validate
TEST_NAME=THROUGHOUT_FAIL make validate
TEST_NAME=WITHIN_PASS make validate
TEST_NAME=WITHIN_FAIL make validate
