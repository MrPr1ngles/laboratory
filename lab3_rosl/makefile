# ---------- Настройки ----------
CXX         := g++
CXXFLAGS    := -std=c++17 -Wall -Wextra \
               -I. \
               -Icore \
               -Iadt \
               -Istorage \
               -Itypes \
               -Iui \
               -Igoogletest/googletest/include
LDFLAGS     := -pthread

BUILD_DIR   := build
GTEST_LIB   := $(BUILD_DIR)/libgtest.a
GTEST_MAIN_OBJ := $(BUILD_DIR)/gtest_main.o
TARGET_UI   := lab3_ui
TARGET_TEST := lab3_test

# ---------- Источники ----------
# UI-версия (main + все файлы внутри ui/ и остальные исходники)
UI_SOURCES := main.cpp \
              $(wildcard ui/*.cpp) \
              $(wildcard core/*.cpp) \
              $(wildcard adt/*.cpp) \
              $(wildcard storage/*.cpp) \
              $(wildcard types/*.cpp)

# Преобразование имён исходников в имена объектных файлов в build/
UI_OBJECTS := $(UI_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# Тесты
TEST_SOURCES := $(wildcard tests/*.cpp)
TEST_OBJECTS := $(TEST_SOURCES:tests/%.cpp=$(BUILD_DIR)/%.test.o)

# GoogleTest sources
GTEST_ALL_SRC := googletest/googletest/src/gtest-all.cc
GTEST_MAIN_SRC := googletest/googletest/src/gtest_main.cc

# ---------- Цели ----------
.PHONY: all clean ui test run run-test

all: ui

ui: $(TARGET_UI)

test: $(TARGET_TEST)

# ---------- Создание папок ----------
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/core $(BUILD_DIR)/adt $(BUILD_DIR)/storage $(BUILD_DIR)/types $(BUILD_DIR)/ui $(BUILD_DIR)/tests

# ---------- GoogleTest ----------
# При компиляции gtest-all.cc требуется видеть папку googletest/googletest,
# т.к. внутри есть includes вида "src/gtest-assertion-result.cc".
$(GTEST_LIB): $(BUILD_DIR)/gtest-all.o
	ar rcs $@ $^

# Компилируем gtest с дополнительным include-путём на googletest root
$(BUILD_DIR)/gtest-all.o: $(GTEST_ALL_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -Igoogletest/googletest -c $< -o $@

# Компилируем gtest_main (определяет main для тестов)
$(GTEST_MAIN_OBJ): $(GTEST_MAIN_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -Igoogletest/googletest -c $< -o $@

# ---------- UI (главная цель) ----------
$(TARGET_UI): $(UI_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Универсальное правило для сборки объектных файлов из .cpp
# Преобразует путь foo/bar.cpp -> build/foo/bar.o
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------- Тесты ----------
# теперь при линковке тестов добавляем object с main (gtest_main.o)
$(TARGET_TEST): $(TEST_OBJECTS) $(GTEST_LIB) $(GTEST_MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# Правило для объектных файлов тестов: tests/X.cpp -> build/X.test.o
$(BUILD_DIR)/%.test.o: tests/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------- Запуск ----------
run: ui
	./$(TARGET_UI)

run-test: test
	./$(TARGET_TEST)

# ---------- Очистка ----------
clean:
	rm -rf $(BUILD_DIR) $(TARGET_UI) $(TARGET_TEST)
