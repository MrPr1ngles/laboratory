#ifndef UI_TYPE_REGISTRY_HPP
#define UI_TYPE_REGISTRY_HPP

// Реестр типов: для каждой записи содержит указатели на create/print/delete/interactive_ops.
// Генерация выполняется через FOR_EACH_TYPE (см. ui/TypeList.hpp).

#include "TypeList.hpp"
#include "ops_templates.hpp"
#include "type_io.hpp"
#include <cstddef>

namespace ui {

using create_stack_fn_t = void* (*)();
using create_deque_fn_t = void* (*)(core::index_t);
using print_stack_fn_t  = void  (*)(void*);
using print_deque_fn_t  = void  (*)(void*);
using delete_stack_fn_t = void  (*)(void*);
using delete_deque_fn_t = void  (*)(void*);
using interactive_ops_fn_t = void (*)(void* /*ADTEntry**/, core::DynamicArray<ADTEntry>&, core::index_t);

// Тип записи реестра
struct TypeOps {
    create_stack_fn_t    create_stack;
    create_deque_fn_t    create_deque;
    print_stack_fn_t     print_stack;
    print_deque_fn_t     print_deque;
    delete_stack_fn_t    delete_stack;
    delete_deque_fn_t    delete_deque;
    interactive_ops_fn_t interactive_ops;
};

// Генерация записей — используем FOR_EACH_TYPE( X ) макрос.
// Для каждого типа T генерируем соответствующие обёртки.

#define REG_ENTRY(ENUM, CPPTYPE) \
    { &ui::create_stack_from_input_generic<CPPTYPE>, &ui::create_deque_from_input_generic<CPPTYPE>, \
      &ui::print_stack_full_generic<CPPTYPE>, &ui::print_deque_full_generic<CPPTYPE>, \
      &ui::delete_stack_t<CPPTYPE>, &ui::delete_deque_t<CPPTYPE>, \
      &ui::interactive_ops_t<CPPTYPE> },

static TypeOps g_type_registry[] = {
    FOR_EACH_TYPE(REG_ENTRY)
};
#undef REG_ENTRY

// Count helper
static const std::size_t g_type_registry_count = sizeof(g_type_registry)/sizeof(g_type_registry[0]);

} // namespace ui

#endif // UI_TYPE_REGISTRY_HPP
