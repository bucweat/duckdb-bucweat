//===----------------------------------------------------------------------===//
// This file is automatically generated by scripts/generate_serialization.py
// Do not edit this file manually, your changes will be overwritten
//===----------------------------------------------------------------------===//

#include "duckdb/common/serializer/serializer.hpp"
#include "duckdb/common/serializer/deserializer.hpp"
#include "duckdb/planner/operator/list.hpp"
#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"

namespace duckdb {

void LogicalOperator::Serialize(Serializer &serializer) const {
	serializer.WriteProperty<LogicalOperatorType>(100, "type", type);
	serializer.WritePropertyWithDefault<vector<unique_ptr<LogicalOperator>>>(101, "children", children);
}

unique_ptr<LogicalOperator> LogicalOperator::Deserialize(Deserializer &deserializer) {
	auto type = deserializer.ReadProperty<LogicalOperatorType>(100, "type");
	auto children = deserializer.ReadPropertyWithDefault<vector<unique_ptr<LogicalOperator>>>(101, "children");
	deserializer.Set<LogicalOperatorType>(type);
	unique_ptr<LogicalOperator> result;
	switch (type) {
	case LogicalOperatorType::LOGICAL_AGGREGATE_AND_GROUP_BY:
		result = LogicalAggregate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_ALTER:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_ANY_JOIN:
		result = LogicalAnyJoin::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_ASOF_JOIN:
		result = LogicalComparisonJoin::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_ATTACH:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CHUNK_GET:
		result = LogicalColumnDataGet::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_COMPARISON_JOIN:
		result = LogicalComparisonJoin::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_COPY_TO_FILE:
		result = LogicalCopyToFile::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_INDEX:
		result = LogicalCreateIndex::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_MACRO:
		result = LogicalCreate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_SCHEMA:
		result = LogicalCreate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_SEQUENCE:
		result = LogicalCreate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_TABLE:
		result = LogicalCreateTable::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_TYPE:
		result = LogicalCreate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CREATE_VIEW:
		result = LogicalCreate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CROSS_PRODUCT:
		result = LogicalCrossProduct::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_CTE_REF:
		result = LogicalCTERef::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DELETE:
		result = LogicalDelete::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DELIM_GET:
		result = LogicalDelimGet::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DELIM_JOIN:
		result = LogicalComparisonJoin::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DETACH:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DISTINCT:
		result = LogicalDistinct::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DROP:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_DUMMY_SCAN:
		result = LogicalDummyScan::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_EMPTY_RESULT:
		result = LogicalEmptyResult::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_EXCEPT:
		result = LogicalSetOperation::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_EXPLAIN:
		result = LogicalExplain::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_EXPRESSION_GET:
		result = LogicalExpressionGet::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_EXTENSION_OPERATOR:
		result = LogicalExtensionOperator::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_FILTER:
		result = LogicalFilter::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_GET:
		result = LogicalGet::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_INSERT:
		result = LogicalInsert::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_INTERSECT:
		result = LogicalSetOperation::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_LIMIT:
		result = LogicalLimit::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_LOAD:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_MATERIALIZED_CTE:
		result = LogicalMaterializedCTE::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_ORDER_BY:
		result = LogicalOrder::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_PIVOT:
		result = LogicalPivot::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_POSITIONAL_JOIN:
		result = LogicalPositionalJoin::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_PROJECTION:
		result = LogicalProjection::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_RECURSIVE_CTE:
		result = LogicalRecursiveCTE::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_RESET:
		result = LogicalReset::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_SAMPLE:
		result = LogicalSample::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_SET:
		result = LogicalSet::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_TOP_N:
		result = LogicalTopN::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_TRANSACTION:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_UNION:
		result = LogicalSetOperation::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_UNNEST:
		result = LogicalUnnest::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_UPDATE:
		result = LogicalUpdate::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_VACUUM:
		result = LogicalSimple::Deserialize(deserializer);
		break;
	case LogicalOperatorType::LOGICAL_WINDOW:
		result = LogicalWindow::Deserialize(deserializer);
		break;
	default:
		throw SerializationException("Unsupported type for deserialization of LogicalOperator!");
	}
	deserializer.Unset<LogicalOperatorType>();
	result->children = std::move(children);
	return result;
}

void FilenamePattern::Serialize(Serializer &serializer) const {
	serializer.WritePropertyWithDefault<string>(200, "base", base);
	serializer.WritePropertyWithDefault<idx_t>(201, "pos", pos);
	serializer.WritePropertyWithDefault<bool>(202, "uuid", uuid);
}

FilenamePattern FilenamePattern::Deserialize(Deserializer &deserializer) {
	FilenamePattern result;
	deserializer.ReadPropertyWithDefault<string>(200, "base", result.base);
	deserializer.ReadPropertyWithDefault<idx_t>(201, "pos", result.pos);
	deserializer.ReadPropertyWithDefault<bool>(202, "uuid", result.uuid);
	return result;
}

void LogicalAggregate::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(200, "expressions", expressions);
	serializer.WritePropertyWithDefault<idx_t>(201, "group_index", group_index);
	serializer.WritePropertyWithDefault<idx_t>(202, "aggregate_index", aggregate_index);
	serializer.WritePropertyWithDefault<idx_t>(203, "groupings_index", groupings_index);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(204, "groups", groups);
	serializer.WritePropertyWithDefault<vector<GroupingSet>>(205, "grouping_sets", grouping_sets);
	serializer.WritePropertyWithDefault<vector<unsafe_vector<idx_t>>>(206, "grouping_functions", grouping_functions);
}

unique_ptr<LogicalOperator> LogicalAggregate::Deserialize(Deserializer &deserializer) {
	auto expressions = deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(200, "expressions");
	auto group_index = deserializer.ReadPropertyWithDefault<idx_t>(201, "group_index");
	auto aggregate_index = deserializer.ReadPropertyWithDefault<idx_t>(202, "aggregate_index");
	auto result = duckdb::unique_ptr<LogicalAggregate>(new LogicalAggregate(group_index, aggregate_index, std::move(expressions)));
	deserializer.ReadPropertyWithDefault<idx_t>(203, "groupings_index", result->groupings_index);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(204, "groups", result->groups);
	deserializer.ReadPropertyWithDefault<vector<GroupingSet>>(205, "grouping_sets", result->grouping_sets);
	deserializer.ReadPropertyWithDefault<vector<unsafe_vector<idx_t>>>(206, "grouping_functions", result->grouping_functions);
	return std::move(result);
}

void LogicalAnyJoin::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WriteProperty<JoinType>(200, "join_type", join_type);
	serializer.WritePropertyWithDefault<idx_t>(201, "mark_index", mark_index);
	serializer.WritePropertyWithDefault<vector<idx_t>>(202, "left_projection_map", left_projection_map);
	serializer.WritePropertyWithDefault<vector<idx_t>>(203, "right_projection_map", right_projection_map);
	serializer.WritePropertyWithDefault<unique_ptr<Expression>>(204, "condition", condition);
}

unique_ptr<LogicalOperator> LogicalAnyJoin::Deserialize(Deserializer &deserializer) {
	auto join_type = deserializer.ReadProperty<JoinType>(200, "join_type");
	auto result = duckdb::unique_ptr<LogicalAnyJoin>(new LogicalAnyJoin(join_type));
	deserializer.ReadPropertyWithDefault<idx_t>(201, "mark_index", result->mark_index);
	deserializer.ReadPropertyWithDefault<vector<idx_t>>(202, "left_projection_map", result->left_projection_map);
	deserializer.ReadPropertyWithDefault<vector<idx_t>>(203, "right_projection_map", result->right_projection_map);
	deserializer.ReadPropertyWithDefault<unique_ptr<Expression>>(204, "condition", result->condition);
	return std::move(result);
}

void LogicalCTERef::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<idx_t>(201, "cte_index", cte_index);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(202, "chunk_types", chunk_types);
	serializer.WritePropertyWithDefault<vector<string>>(203, "bound_columns", bound_columns);
	serializer.WriteProperty<CTEMaterialize>(204, "materialized_cte", materialized_cte);
}

unique_ptr<LogicalOperator> LogicalCTERef::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto cte_index = deserializer.ReadPropertyWithDefault<idx_t>(201, "cte_index");
	auto chunk_types = deserializer.ReadPropertyWithDefault<vector<LogicalType>>(202, "chunk_types");
	auto bound_columns = deserializer.ReadPropertyWithDefault<vector<string>>(203, "bound_columns");
	auto materialized_cte = deserializer.ReadProperty<CTEMaterialize>(204, "materialized_cte");
	auto result = duckdb::unique_ptr<LogicalCTERef>(new LogicalCTERef(table_index, cte_index, std::move(chunk_types), std::move(bound_columns), materialized_cte));
	return std::move(result);
}

void LogicalColumnDataGet::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(201, "chunk_types", chunk_types);
	serializer.WritePropertyWithDefault<unique_ptr<ColumnDataCollection>>(202, "collection", collection);
}

unique_ptr<LogicalOperator> LogicalColumnDataGet::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto chunk_types = deserializer.ReadPropertyWithDefault<vector<LogicalType>>(201, "chunk_types");
	auto collection = deserializer.ReadPropertyWithDefault<unique_ptr<ColumnDataCollection>>(202, "collection");
	auto result = duckdb::unique_ptr<LogicalColumnDataGet>(new LogicalColumnDataGet(table_index, std::move(chunk_types), std::move(collection)));
	return std::move(result);
}

void LogicalComparisonJoin::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WriteProperty<JoinType>(200, "join_type", join_type);
	serializer.WritePropertyWithDefault<idx_t>(201, "mark_index", mark_index);
	serializer.WritePropertyWithDefault<vector<idx_t>>(202, "left_projection_map", left_projection_map);
	serializer.WritePropertyWithDefault<vector<idx_t>>(203, "right_projection_map", right_projection_map);
	serializer.WritePropertyWithDefault<vector<JoinCondition>>(204, "conditions", conditions);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(205, "mark_types", mark_types);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(206, "duplicate_eliminated_columns", duplicate_eliminated_columns);
	serializer.WritePropertyWithDefault<bool>(207, "delim_flipped", delim_flipped, false);
}

unique_ptr<LogicalOperator> LogicalComparisonJoin::Deserialize(Deserializer &deserializer) {
	auto join_type = deserializer.ReadProperty<JoinType>(200, "join_type");
	auto result = duckdb::unique_ptr<LogicalComparisonJoin>(new LogicalComparisonJoin(join_type, deserializer.Get<LogicalOperatorType>()));
	deserializer.ReadPropertyWithDefault<idx_t>(201, "mark_index", result->mark_index);
	deserializer.ReadPropertyWithDefault<vector<idx_t>>(202, "left_projection_map", result->left_projection_map);
	deserializer.ReadPropertyWithDefault<vector<idx_t>>(203, "right_projection_map", result->right_projection_map);
	deserializer.ReadPropertyWithDefault<vector<JoinCondition>>(204, "conditions", result->conditions);
	deserializer.ReadPropertyWithDefault<vector<LogicalType>>(205, "mark_types", result->mark_types);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(206, "duplicate_eliminated_columns", result->duplicate_eliminated_columns);
	deserializer.ReadPropertyWithDefault<bool>(207, "delim_flipped", result->delim_flipped, false);
	return std::move(result);
}

void LogicalCreate::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<CreateInfo>>(200, "info", info);
}

unique_ptr<LogicalOperator> LogicalCreate::Deserialize(Deserializer &deserializer) {
	auto info = deserializer.ReadPropertyWithDefault<unique_ptr<CreateInfo>>(200, "info");
	auto result = duckdb::unique_ptr<LogicalCreate>(new LogicalCreate(deserializer.Get<LogicalOperatorType>(), deserializer.Get<ClientContext &>(), std::move(info)));
	return std::move(result);
}

void LogicalCreateIndex::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<CreateIndexInfo>>(200, "info", info);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(201, "unbound_expressions", unbound_expressions);
}

unique_ptr<LogicalOperator> LogicalCreateIndex::Deserialize(Deserializer &deserializer) {
	auto info = deserializer.ReadPropertyWithDefault<unique_ptr<CreateInfo>>(200, "info");
	auto unbound_expressions = deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(201, "unbound_expressions");
	auto result = duckdb::unique_ptr<LogicalCreateIndex>(new LogicalCreateIndex(deserializer.Get<ClientContext &>(), std::move(info), std::move(unbound_expressions)));
	return std::move(result);
}

void LogicalCreateTable::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<CreateInfo>>(200, "info", info->base);
}

unique_ptr<LogicalOperator> LogicalCreateTable::Deserialize(Deserializer &deserializer) {
	auto info = deserializer.ReadPropertyWithDefault<unique_ptr<CreateInfo>>(200, "info");
	auto result = duckdb::unique_ptr<LogicalCreateTable>(new LogicalCreateTable(deserializer.Get<ClientContext &>(), std::move(info)));
	return std::move(result);
}

void LogicalCrossProduct::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
}

unique_ptr<LogicalOperator> LogicalCrossProduct::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalCrossProduct>(new LogicalCrossProduct());
	return std::move(result);
}

void LogicalDelete::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<CreateInfo>>(200, "table_info", table.GetInfo());
	serializer.WritePropertyWithDefault<idx_t>(201, "table_index", table_index);
	serializer.WritePropertyWithDefault<bool>(202, "return_chunk", return_chunk);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(203, "expressions", expressions);
}

unique_ptr<LogicalOperator> LogicalDelete::Deserialize(Deserializer &deserializer) {
	auto table_info = deserializer.ReadPropertyWithDefault<unique_ptr<CreateInfo>>(200, "table_info");
	auto result = duckdb::unique_ptr<LogicalDelete>(new LogicalDelete(deserializer.Get<ClientContext &>(), table_info));
	deserializer.ReadPropertyWithDefault<idx_t>(201, "table_index", result->table_index);
	deserializer.ReadPropertyWithDefault<bool>(202, "return_chunk", result->return_chunk);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(203, "expressions", result->expressions);
	return std::move(result);
}

void LogicalDelimGet::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(201, "chunk_types", chunk_types);
}

unique_ptr<LogicalOperator> LogicalDelimGet::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto chunk_types = deserializer.ReadPropertyWithDefault<vector<LogicalType>>(201, "chunk_types");
	auto result = duckdb::unique_ptr<LogicalDelimGet>(new LogicalDelimGet(table_index, std::move(chunk_types)));
	return std::move(result);
}

void LogicalDistinct::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WriteProperty<DistinctType>(200, "distinct_type", distinct_type);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(201, "distinct_targets", distinct_targets);
	serializer.WritePropertyWithDefault<unique_ptr<BoundOrderModifier>>(202, "order_by", order_by);
}

unique_ptr<LogicalOperator> LogicalDistinct::Deserialize(Deserializer &deserializer) {
	auto distinct_type = deserializer.ReadProperty<DistinctType>(200, "distinct_type");
	auto distinct_targets = deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(201, "distinct_targets");
	auto result = duckdb::unique_ptr<LogicalDistinct>(new LogicalDistinct(std::move(distinct_targets), distinct_type));
	deserializer.ReadPropertyWithDefault<unique_ptr<BoundOrderModifier>>(202, "order_by", result->order_by);
	return std::move(result);
}

void LogicalDummyScan::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
}

unique_ptr<LogicalOperator> LogicalDummyScan::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto result = duckdb::unique_ptr<LogicalDummyScan>(new LogicalDummyScan(table_index));
	return std::move(result);
}

void LogicalEmptyResult::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(200, "return_types", return_types);
	serializer.WritePropertyWithDefault<vector<ColumnBinding>>(201, "bindings", bindings);
}

unique_ptr<LogicalOperator> LogicalEmptyResult::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalEmptyResult>(new LogicalEmptyResult());
	deserializer.ReadPropertyWithDefault<vector<LogicalType>>(200, "return_types", result->return_types);
	deserializer.ReadPropertyWithDefault<vector<ColumnBinding>>(201, "bindings", result->bindings);
	return std::move(result);
}

void LogicalExplain::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WriteProperty<ExplainType>(200, "explain_type", explain_type);
	serializer.WritePropertyWithDefault<string>(201, "physical_plan", physical_plan);
	serializer.WritePropertyWithDefault<string>(202, "logical_plan_unopt", logical_plan_unopt);
	serializer.WritePropertyWithDefault<string>(203, "logical_plan_opt", logical_plan_opt);
}

unique_ptr<LogicalOperator> LogicalExplain::Deserialize(Deserializer &deserializer) {
	auto explain_type = deserializer.ReadProperty<ExplainType>(200, "explain_type");
	auto result = duckdb::unique_ptr<LogicalExplain>(new LogicalExplain(explain_type));
	deserializer.ReadPropertyWithDefault<string>(201, "physical_plan", result->physical_plan);
	deserializer.ReadPropertyWithDefault<string>(202, "logical_plan_unopt", result->logical_plan_unopt);
	deserializer.ReadPropertyWithDefault<string>(203, "logical_plan_opt", result->logical_plan_opt);
	return std::move(result);
}

void LogicalExpressionGet::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(201, "expr_types", expr_types);
	serializer.WritePropertyWithDefault<vector<vector<unique_ptr<Expression>>>>(202, "expressions", expressions);
}

unique_ptr<LogicalOperator> LogicalExpressionGet::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto expr_types = deserializer.ReadPropertyWithDefault<vector<LogicalType>>(201, "expr_types");
	auto expressions = deserializer.ReadPropertyWithDefault<vector<vector<unique_ptr<Expression>>>>(202, "expressions");
	auto result = duckdb::unique_ptr<LogicalExpressionGet>(new LogicalExpressionGet(table_index, std::move(expr_types), std::move(expressions)));
	return std::move(result);
}

void LogicalFilter::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(200, "expressions", expressions);
	serializer.WritePropertyWithDefault<vector<idx_t>>(201, "projection_map", projection_map);
}

unique_ptr<LogicalOperator> LogicalFilter::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalFilter>(new LogicalFilter());
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(200, "expressions", result->expressions);
	deserializer.ReadPropertyWithDefault<vector<idx_t>>(201, "projection_map", result->projection_map);
	return std::move(result);
}

void LogicalInsert::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<CreateInfo>>(200, "table_info", table.GetInfo());
	serializer.WritePropertyWithDefault<vector<vector<unique_ptr<Expression>>>>(201, "insert_values", insert_values);
	serializer.WriteProperty<IndexVector<idx_t, PhysicalIndex>>(202, "column_index_map", column_index_map);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(203, "expected_types", expected_types);
	serializer.WritePropertyWithDefault<idx_t>(204, "table_index", table_index);
	serializer.WritePropertyWithDefault<bool>(205, "return_chunk", return_chunk);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(206, "bound_defaults", bound_defaults);
	serializer.WriteProperty<OnConflictAction>(207, "action_type", action_type);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(208, "expected_set_types", expected_set_types);
	serializer.WritePropertyWithDefault<unordered_set<idx_t>>(209, "on_conflict_filter", on_conflict_filter);
	serializer.WritePropertyWithDefault<unique_ptr<Expression>>(210, "on_conflict_condition", on_conflict_condition);
	serializer.WritePropertyWithDefault<unique_ptr<Expression>>(211, "do_update_condition", do_update_condition);
	serializer.WritePropertyWithDefault<vector<PhysicalIndex>>(212, "set_columns", set_columns);
	serializer.WritePropertyWithDefault<vector<LogicalType>>(213, "set_types", set_types);
	serializer.WritePropertyWithDefault<idx_t>(214, "excluded_table_index", excluded_table_index);
	serializer.WritePropertyWithDefault<vector<column_t>>(215, "columns_to_fetch", columns_to_fetch);
	serializer.WritePropertyWithDefault<vector<column_t>>(216, "source_columns", source_columns);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(217, "expressions", expressions);
}

unique_ptr<LogicalOperator> LogicalInsert::Deserialize(Deserializer &deserializer) {
	auto table_info = deserializer.ReadPropertyWithDefault<unique_ptr<CreateInfo>>(200, "table_info");
	auto result = duckdb::unique_ptr<LogicalInsert>(new LogicalInsert(deserializer.Get<ClientContext &>(), std::move(table_info)));
	deserializer.ReadPropertyWithDefault<vector<vector<unique_ptr<Expression>>>>(201, "insert_values", result->insert_values);
	deserializer.ReadProperty<IndexVector<idx_t, PhysicalIndex>>(202, "column_index_map", result->column_index_map);
	deserializer.ReadPropertyWithDefault<vector<LogicalType>>(203, "expected_types", result->expected_types);
	deserializer.ReadPropertyWithDefault<idx_t>(204, "table_index", result->table_index);
	deserializer.ReadPropertyWithDefault<bool>(205, "return_chunk", result->return_chunk);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(206, "bound_defaults", result->bound_defaults);
	deserializer.ReadProperty<OnConflictAction>(207, "action_type", result->action_type);
	deserializer.ReadPropertyWithDefault<vector<LogicalType>>(208, "expected_set_types", result->expected_set_types);
	deserializer.ReadPropertyWithDefault<unordered_set<idx_t>>(209, "on_conflict_filter", result->on_conflict_filter);
	deserializer.ReadPropertyWithDefault<unique_ptr<Expression>>(210, "on_conflict_condition", result->on_conflict_condition);
	deserializer.ReadPropertyWithDefault<unique_ptr<Expression>>(211, "do_update_condition", result->do_update_condition);
	deserializer.ReadPropertyWithDefault<vector<PhysicalIndex>>(212, "set_columns", result->set_columns);
	deserializer.ReadPropertyWithDefault<vector<LogicalType>>(213, "set_types", result->set_types);
	deserializer.ReadPropertyWithDefault<idx_t>(214, "excluded_table_index", result->excluded_table_index);
	deserializer.ReadPropertyWithDefault<vector<column_t>>(215, "columns_to_fetch", result->columns_to_fetch);
	deserializer.ReadPropertyWithDefault<vector<column_t>>(216, "source_columns", result->source_columns);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(217, "expressions", result->expressions);
	return std::move(result);
}

void LogicalLimit::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WriteProperty<BoundLimitNode>(200, "limit_val", limit_val);
	serializer.WriteProperty<BoundLimitNode>(201, "offset_val", offset_val);
}

unique_ptr<LogicalOperator> LogicalLimit::Deserialize(Deserializer &deserializer) {
	auto limit_val = deserializer.ReadProperty<BoundLimitNode>(200, "limit_val");
	auto offset_val = deserializer.ReadProperty<BoundLimitNode>(201, "offset_val");
	auto result = duckdb::unique_ptr<LogicalLimit>(new LogicalLimit(std::move(limit_val), std::move(offset_val)));
	return std::move(result);
}

void LogicalMaterializedCTE::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<idx_t>(201, "column_count", column_count);
	serializer.WritePropertyWithDefault<string>(202, "ctename", ctename);
}

unique_ptr<LogicalOperator> LogicalMaterializedCTE::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalMaterializedCTE>(new LogicalMaterializedCTE());
	deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index", result->table_index);
	deserializer.ReadPropertyWithDefault<idx_t>(201, "column_count", result->column_count);
	deserializer.ReadPropertyWithDefault<string>(202, "ctename", result->ctename);
	return std::move(result);
}

void LogicalOrder::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<vector<BoundOrderByNode>>(200, "orders", orders);
	serializer.WritePropertyWithDefault<vector<idx_t>>(201, "projections", projections);
}

unique_ptr<LogicalOperator> LogicalOrder::Deserialize(Deserializer &deserializer) {
	auto orders = deserializer.ReadPropertyWithDefault<vector<BoundOrderByNode>>(200, "orders");
	auto result = duckdb::unique_ptr<LogicalOrder>(new LogicalOrder(std::move(orders)));
	deserializer.ReadPropertyWithDefault<vector<idx_t>>(201, "projections", result->projections);
	return std::move(result);
}

void LogicalPivot::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "pivot_index", pivot_index);
	serializer.WriteProperty<BoundPivotInfo>(201, "bound_pivot", bound_pivot);
}

unique_ptr<LogicalOperator> LogicalPivot::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalPivot>(new LogicalPivot());
	deserializer.ReadPropertyWithDefault<idx_t>(200, "pivot_index", result->pivot_index);
	deserializer.ReadProperty<BoundPivotInfo>(201, "bound_pivot", result->bound_pivot);
	return std::move(result);
}

void LogicalPositionalJoin::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
}

unique_ptr<LogicalOperator> LogicalPositionalJoin::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalPositionalJoin>(new LogicalPositionalJoin());
	return std::move(result);
}

void LogicalProjection::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(201, "expressions", expressions);
}

unique_ptr<LogicalOperator> LogicalProjection::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto expressions = deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(201, "expressions");
	auto result = duckdb::unique_ptr<LogicalProjection>(new LogicalProjection(table_index, std::move(expressions)));
	return std::move(result);
}

void LogicalRecursiveCTE::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<bool>(200, "union_all", union_all);
	serializer.WritePropertyWithDefault<string>(201, "ctename", ctename);
	serializer.WritePropertyWithDefault<idx_t>(202, "table_index", table_index);
	serializer.WritePropertyWithDefault<idx_t>(203, "column_count", column_count);
}

unique_ptr<LogicalOperator> LogicalRecursiveCTE::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalRecursiveCTE>(new LogicalRecursiveCTE());
	deserializer.ReadPropertyWithDefault<bool>(200, "union_all", result->union_all);
	deserializer.ReadPropertyWithDefault<string>(201, "ctename", result->ctename);
	deserializer.ReadPropertyWithDefault<idx_t>(202, "table_index", result->table_index);
	deserializer.ReadPropertyWithDefault<idx_t>(203, "column_count", result->column_count);
	return std::move(result);
}

void LogicalReset::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<string>(200, "name", name);
	serializer.WriteProperty<SetScope>(201, "scope", scope);
}

unique_ptr<LogicalOperator> LogicalReset::Deserialize(Deserializer &deserializer) {
	auto name = deserializer.ReadPropertyWithDefault<string>(200, "name");
	auto scope = deserializer.ReadProperty<SetScope>(201, "scope");
	auto result = duckdb::unique_ptr<LogicalReset>(new LogicalReset(std::move(name), scope));
	return std::move(result);
}

void LogicalSample::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<SampleOptions>>(200, "sample_options", sample_options);
}

unique_ptr<LogicalOperator> LogicalSample::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<LogicalSample>(new LogicalSample());
	deserializer.ReadPropertyWithDefault<unique_ptr<SampleOptions>>(200, "sample_options", result->sample_options);
	return std::move(result);
}

void LogicalSet::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<string>(200, "name", name);
	serializer.WriteProperty<Value>(201, "value", value);
	serializer.WriteProperty<SetScope>(202, "scope", scope);
}

unique_ptr<LogicalOperator> LogicalSet::Deserialize(Deserializer &deserializer) {
	auto name = deserializer.ReadPropertyWithDefault<string>(200, "name");
	auto value = deserializer.ReadProperty<Value>(201, "value");
	auto scope = deserializer.ReadProperty<SetScope>(202, "scope");
	auto result = duckdb::unique_ptr<LogicalSet>(new LogicalSet(std::move(name), value, scope));
	return std::move(result);
}

void LogicalSetOperation::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "table_index", table_index);
	serializer.WritePropertyWithDefault<idx_t>(201, "column_count", column_count);
	serializer.WritePropertyWithDefault<bool>(202, "setop_all", setop_all, true);
	serializer.WritePropertyWithDefault<bool>(203, "allow_out_of_order", allow_out_of_order, true);
}

unique_ptr<LogicalOperator> LogicalSetOperation::Deserialize(Deserializer &deserializer) {
	auto table_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "table_index");
	auto column_count = deserializer.ReadPropertyWithDefault<idx_t>(201, "column_count");
	auto setop_all = deserializer.ReadPropertyWithDefault<bool>(202, "setop_all", true);
	auto allow_out_of_order = deserializer.ReadPropertyWithDefault<bool>(203, "allow_out_of_order", true);
	auto result = duckdb::unique_ptr<LogicalSetOperation>(new LogicalSetOperation(table_index, column_count, deserializer.Get<LogicalOperatorType>(), setop_all, allow_out_of_order));
	return std::move(result);
}

void LogicalSimple::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<ParseInfo>>(200, "info", info);
}

unique_ptr<LogicalOperator> LogicalSimple::Deserialize(Deserializer &deserializer) {
	auto info = deserializer.ReadPropertyWithDefault<unique_ptr<ParseInfo>>(200, "info");
	auto result = duckdb::unique_ptr<LogicalSimple>(new LogicalSimple(deserializer.Get<LogicalOperatorType>(), std::move(info)));
	return std::move(result);
}

void LogicalTopN::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<vector<BoundOrderByNode>>(200, "orders", orders);
	serializer.WritePropertyWithDefault<idx_t>(201, "limit", limit);
	serializer.WritePropertyWithDefault<idx_t>(202, "offset", offset);
}

unique_ptr<LogicalOperator> LogicalTopN::Deserialize(Deserializer &deserializer) {
	auto orders = deserializer.ReadPropertyWithDefault<vector<BoundOrderByNode>>(200, "orders");
	auto limit = deserializer.ReadPropertyWithDefault<idx_t>(201, "limit");
	auto offset = deserializer.ReadPropertyWithDefault<idx_t>(202, "offset");
	auto result = duckdb::unique_ptr<LogicalTopN>(new LogicalTopN(std::move(orders), limit, offset));
	return std::move(result);
}

void LogicalUnnest::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "unnest_index", unnest_index);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(201, "expressions", expressions);
}

unique_ptr<LogicalOperator> LogicalUnnest::Deserialize(Deserializer &deserializer) {
	auto unnest_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "unnest_index");
	auto result = duckdb::unique_ptr<LogicalUnnest>(new LogicalUnnest(unnest_index));
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(201, "expressions", result->expressions);
	return std::move(result);
}

void LogicalUpdate::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<unique_ptr<CreateInfo>>(200, "table_info", table.GetInfo());
	serializer.WritePropertyWithDefault<idx_t>(201, "table_index", table_index);
	serializer.WritePropertyWithDefault<bool>(202, "return_chunk", return_chunk);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(203, "expressions", expressions);
	serializer.WritePropertyWithDefault<vector<PhysicalIndex>>(204, "columns", columns);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(205, "bound_defaults", bound_defaults);
	serializer.WritePropertyWithDefault<bool>(206, "update_is_del_and_insert", update_is_del_and_insert);
}

unique_ptr<LogicalOperator> LogicalUpdate::Deserialize(Deserializer &deserializer) {
	auto table_info = deserializer.ReadPropertyWithDefault<unique_ptr<CreateInfo>>(200, "table_info");
	auto result = duckdb::unique_ptr<LogicalUpdate>(new LogicalUpdate(deserializer.Get<ClientContext &>(), table_info));
	deserializer.ReadPropertyWithDefault<idx_t>(201, "table_index", result->table_index);
	deserializer.ReadPropertyWithDefault<bool>(202, "return_chunk", result->return_chunk);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(203, "expressions", result->expressions);
	deserializer.ReadPropertyWithDefault<vector<PhysicalIndex>>(204, "columns", result->columns);
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(205, "bound_defaults", result->bound_defaults);
	deserializer.ReadPropertyWithDefault<bool>(206, "update_is_del_and_insert", result->update_is_del_and_insert);
	return std::move(result);
}

void LogicalWindow::Serialize(Serializer &serializer) const {
	LogicalOperator::Serialize(serializer);
	serializer.WritePropertyWithDefault<idx_t>(200, "window_index", window_index);
	serializer.WritePropertyWithDefault<vector<unique_ptr<Expression>>>(201, "expressions", expressions);
}

unique_ptr<LogicalOperator> LogicalWindow::Deserialize(Deserializer &deserializer) {
	auto window_index = deserializer.ReadPropertyWithDefault<idx_t>(200, "window_index");
	auto result = duckdb::unique_ptr<LogicalWindow>(new LogicalWindow(window_index));
	deserializer.ReadPropertyWithDefault<vector<unique_ptr<Expression>>>(201, "expressions", result->expressions);
	return std::move(result);
}

} // namespace duckdb
