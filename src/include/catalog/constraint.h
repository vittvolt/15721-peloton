//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// constraint.h
//
// Identification: src/include/catalog/constraint.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


#pragma once

#include <string>
#include <vector>

#include "common/printable.h"
#include "type/type.h"
#include "type/types.h"

namespace peloton {
namespace catalog {

//===--------------------------------------------------------------------===//
// Constraint Class
//===--------------------------------------------------------------------===//

class Constraint : public Printable {
 public:
  Constraint(ConstraintType type, std::string constraint_name, 
             std::vector<oid_t> column_ids)
      : constraint_type(type), constraint_name(constraint_name),
        column_ids(column_ids) {}

  //===--------------------------------------------------------------------===//
  // ACCESSORS
  //===--------------------------------------------------------------------===//

  ConstraintType GetType() const { return constraint_type; }

  // Offset into the list of "reference tables" in the Table.
  void SetForeignKeyListOffset(oid_t offset) { fk_list_offset = offset; }

  // Offset into the list of "unique indices" in the Table.
  void SetUniqueIndexOffset(oid_t offset) { unique_index_list_offset = offset; }

  // Set default value. Only applies if DEFAULT constraint
  void SetDefault(type::Value *default_value) {
    if (constraint_type == ConstraintType::DEFAULT) {
      default_value = default_value;
    }
  }

  // Set check expression. Only applies if CHECK constraint
  void SetCheck(ExpressionType op, type::Value val) {
    if (constraint_type == ConstraintType::CHECK) {
      exp = std::pair<ExpressionType, type::Value>(op, val);
    }
  }

  // Get the default value type
  type::Value GetDefault() const { return default_value; }

  // Get the check expression
  std::pair<ExpressionType, type::Value> GetCheck() { return check_exp; }

  // Get the offset
  oid_t GetForeignKeyListOffset() const { return fk_list_offset; }

  // Get the offset
  oid_t GetUniqueIndexOffset() const { return unique_index_list_offset; }

  std::string GetName() const { return constraint_name; }

  // Get a string representation for debugging
  const std::string GetInfo() const;

  // Get the offsets of the columns covered by this constraint
  std::vector<oid_t> &GetColumnIDs() const { return column_ids; }

 private:
  //===--------------------------------------------------------------------===//
  // MEMBERS
  //===--------------------------------------------------------------------===//

  // The type of constraint
  ConstraintType constraint_type = ConstraintType::INVALID;

  // Offsets into the Unique index and reference table lists in Table
  oid_t fk_list_offset = INVALID_OID;

  oid_t unique_index_list_offset = INVALID_OID;

  std::string constraint_name;

  std::vector<oid_t> column_ids;

  type::Value *default_value = nullptr;

  std::pair<ExpressionType, type::Value> check_exp;
};

}  // End catalog namespace
}  // End peloton namespace
