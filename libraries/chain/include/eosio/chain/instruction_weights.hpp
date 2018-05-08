#pragma once

#include "multi_index_includes.hpp"

namespace eosio { namespace chain {
   enum class native_weighting_type : uint16_t {
      intrinsic_base = 0,
      per_byte,
      WEIGHTS_SIZE,
      BEGIN = intrinsic_base,
      END   = per_byte
   };
   enum class wasm_weighting_type : uint16_t {
      softfloat32_ops = 0,
      softfloat64_ops,
      builtins_f128_ops,
      builtins_i128_ops,
      WEIGHTS_SIZE,
      BEGIN = softfloat32_ops,
      END   = builtins_i128_ops
   };

   template< typename T >
   class enum_wrapper {
   public:
      class iterator {
      public:
         iterator( typename std::underlying_type<T>::type value ) : _val( value ) { }

         T operator*( void ) const {
            return static_cast<T>(_val);
         }

         void operator++( void )
         {
            ++_val;
         }

         bool operator!=( iterator rhs ) {
            return _val != rhs._val;
         }

      private:
         typename std::underlying_type<T>::type _val;
      };

   };
   template< typename T >
   typename enum_wrapper<T>::iterator begin( enum_wrapper<T> ) {
      return typename enum_wrapper<T>::iterator( static_cast<typename std::underlying_type<T>::type>(T::BEGIN) );
   }
   template< typename T >
   typename enum_wrapper<T>::iterator end( enum_wrapper<T> ) {
      return typename enum_wrapper<T>::iterator( (static_cast<typename std::underlying_type<T>::type>(T::END) + 1 ));
   }
   
   struct native_instruction_weights {
      uint16_t base_intrinsic = 3;
      uint16_t per_byte = 2;
      uint32_t version = 0;
   };

   struct wasm_instruction_weights {
      uint16_t softfloat32_ops = 7;
      uint16_t softfloat64_ops = 9;
      uint16_t builtinsf128_ops = 11;
      uint16_t builtinsi128_ops = 5;
      uint32_t version = 0;
   };

   class native_instruction_weight_object : public chainbase::object<native_instruction_weight_object_type, native_instruction_weight_object> {
      OBJECT_CTOR(native_instruction_weight_object)
      id_type  id;
      optional<native_instruction_weights> proposed_weights;
      block_num_type ref_block;
      native_instruction_weights weights;
   };
   
   struct by_ref_block_type;
   using native_instruction_weight_index = chainbase::shared_multi_index_container<
      native_instruction_weight_object,
      indexed_by<
         ordered_unique<tag<by_id>, member<native_instruction_weight_object, native_instruction_weight_object::id_type, &native_instruction_weight_object::id>>,
         ordered_unique<tag<by_ref_block_type>, member<native_instruction_weight_object, block_num_type, &native_instruction_weight_object::ref_block>>
      >
   >;
   
}} // namespace eosio chain

CHAINBASE_SET_INDEX_TYPE(eosio::chain::native_instruction_weight_object, eosio::chain::native_instruction_weight_index)
FC_REFLECT(eosio::chain::native_instruction_weights, (base_intrinsic)(per_byte)(version))
FC_REFLECT(eosio::chain::wasm_instruction_weights, (softfloat32_ops)(softfloat64_ops)(builtinsf128_ops)(builtinsi128_ops)(version))
FC_REFLECT(eosio::chain::native_instruction_weight_object, (proposed_weights)(ref_block)(weights))
