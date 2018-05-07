#pragma once

#include "multi_index_includes.hpp"

namespace eosio { namespace chain {
   enum class weighting_type : uint16_t {
      softfloat32_ops = 0,
      softfloat64_ops,
      builtins_f128_ops,
      builtins_i128_ops,
      intrinsic_base,
      per_byte,
      WEIGHTS_SIZE,
      BEGIN = softfloat32_ops,
      END   = per_byte
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

   class instruction_weight_object : public chainbase::object<instruction_weight_object_type, instruction_weight_object> {
      OBJECT_CTOR(instruction_weight_object)

      id_type  id;
      weighting_type type;
      uint16_t weight;
   };
   
   struct by_weighting_type;
   using instruction_weight_index = chainbase::shared_multi_index_container<
      instruction_weight_object,
      indexed_by<
         ordered_unique<tag<by_id>, member<instruction_weight_object, instruction_weight_object::id_type, &instruction_weight_object::id>>,
         ordered_unique<tag<by_weighting_type>, member<instruction_weight_object, weighting_type, &instruction_weight_object::type>>
      >
   >;

   class instruction_weights {
      public:
         struct defaults {
            uint16_t weights[static_cast<uint16_t>(weighting_type::WEIGHTS_SIZE)] = {
               7, /* softfloat32 */
               9, /* softfloat64 */
               11, /* builtins_f128 */
               5, /* builtins_i128 */
               3, /* base instrinsic */
               2  /* per byte */
            }; 
         };
         explicit instruction_weights( chainbase::database& db ) : _db(db) {
            for ( auto wt : enum_wrapper<weighting_type>() ) {
               _weights[static_cast<uint16_t>(wt)] = _db.get<instruction_weight_object, by_weighting_type>( weighting_type::softfloat32_ops ).weight;
            }
         }
         inline uint16_t get_weight( const weighting_type wt ) const {
            return _weights[static_cast<uint16_t>(wt)];
         }

      private:
         chainbase::database& _db;
         uint16_t _weights[static_cast<uint16_t>(weighting_type::WEIGHTS_SIZE)];

   };

}} // namespace eosio chain

CHAINBASE_SET_INDEX_TYPE(eosio::chain::instruction_weight_object, eosio::chain::instruction_weight_index)

FC_REFLECT(eosio::chain::instruction_weight_object, (type)(weight))
