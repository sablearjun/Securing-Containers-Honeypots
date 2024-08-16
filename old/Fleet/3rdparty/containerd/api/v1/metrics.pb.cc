// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: metrics.proto

#include "metrics.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace containerd {
namespace types {
constexpr Metric::Metric(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : id_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , timestamp_(nullptr)
  , data_(nullptr){}
struct MetricDefaultTypeInternal {
  constexpr MetricDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~MetricDefaultTypeInternal() {}
  union {
    Metric _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT MetricDefaultTypeInternal _Metric_default_instance_;
}  // namespace types
}  // namespace containerd
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_metrics_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_metrics_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_metrics_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_metrics_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::containerd::types::Metric, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::containerd::types::Metric, timestamp_),
  PROTOBUF_FIELD_OFFSET(::containerd::types::Metric, id_),
  PROTOBUF_FIELD_OFFSET(::containerd::types::Metric, data_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::containerd::types::Metric)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::containerd::types::_Metric_default_instance_),
};

const char descriptor_table_protodef_metrics_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rmetrics.proto\022\020containerd.types\032\ngogo."
  "proto\032\031google/protobuf/any.proto\032\037google"
  "/protobuf/timestamp.proto\"q\n\006Metric\0227\n\tt"
  "imestamp\030\001 \001(\0132\032.google.protobuf.Timesta"
  "mpB\010\220\337\037\001\310\336\037\000\022\n\n\002id\030\002 \001(\t\022\"\n\004data\030\003 \001(\0132\024"
  ".google.protobuf.AnyB2Z0github.com/conta"
  "inerd/containerd/api/types;typesb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_metrics_2eproto_deps[3] = {
  &::descriptor_table_gogo_2eproto,
  &::descriptor_table_google_2fprotobuf_2fany_2eproto,
  &::descriptor_table_google_2fprotobuf_2ftimestamp_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_metrics_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_metrics_2eproto = {
  false, false, 280, descriptor_table_protodef_metrics_2eproto, "metrics.proto", 
  &descriptor_table_metrics_2eproto_once, descriptor_table_metrics_2eproto_deps, 3, 1,
  schemas, file_default_instances, TableStruct_metrics_2eproto::offsets,
  file_level_metadata_metrics_2eproto, file_level_enum_descriptors_metrics_2eproto, file_level_service_descriptors_metrics_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_metrics_2eproto_getter() {
  return &descriptor_table_metrics_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_metrics_2eproto(&descriptor_table_metrics_2eproto);
namespace containerd {
namespace types {

// ===================================================================

class Metric::_Internal {
 public:
  static const PROTOBUF_NAMESPACE_ID::Timestamp& timestamp(const Metric* msg);
  static const PROTOBUF_NAMESPACE_ID::Any& data(const Metric* msg);
};

const PROTOBUF_NAMESPACE_ID::Timestamp&
Metric::_Internal::timestamp(const Metric* msg) {
  return *msg->timestamp_;
}
const PROTOBUF_NAMESPACE_ID::Any&
Metric::_Internal::data(const Metric* msg) {
  return *msg->data_;
}
void Metric::clear_timestamp() {
  if (GetArenaForAllocation() == nullptr && timestamp_ != nullptr) {
    delete timestamp_;
  }
  timestamp_ = nullptr;
}
void Metric::clear_data() {
  if (GetArenaForAllocation() == nullptr && data_ != nullptr) {
    delete data_;
  }
  data_ = nullptr;
}
Metric::Metric(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:containerd.types.Metric)
}
Metric::Metric(const Metric& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_id().empty()) {
    id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_id(), 
      GetArenaForAllocation());
  }
  if (from._internal_has_timestamp()) {
    timestamp_ = new PROTOBUF_NAMESPACE_ID::Timestamp(*from.timestamp_);
  } else {
    timestamp_ = nullptr;
  }
  if (from._internal_has_data()) {
    data_ = new PROTOBUF_NAMESPACE_ID::Any(*from.data_);
  } else {
    data_ = nullptr;
  }
  // @@protoc_insertion_point(copy_constructor:containerd.types.Metric)
}

inline void Metric::SharedCtor() {
id_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&timestamp_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&data_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(data_));
}

Metric::~Metric() {
  // @@protoc_insertion_point(destructor:containerd.types.Metric)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void Metric::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  id_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) delete timestamp_;
  if (this != internal_default_instance()) delete data_;
}

void Metric::ArenaDtor(void* object) {
  Metric* _this = reinterpret_cast< Metric* >(object);
  (void)_this;
}
void Metric::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Metric::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Metric::Clear() {
// @@protoc_insertion_point(message_clear_start:containerd.types.Metric)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  id_.ClearToEmpty();
  if (GetArenaForAllocation() == nullptr && timestamp_ != nullptr) {
    delete timestamp_;
  }
  timestamp_ = nullptr;
  if (GetArenaForAllocation() == nullptr && data_ != nullptr) {
    delete data_;
  }
  data_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Metric::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .google.protobuf.Timestamp timestamp = 1 [(.gogoproto.nullable) = false, (.gogoproto.stdtime) = true];
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_timestamp(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_id();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "containerd.types.Metric.id"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .google.protobuf.Any data = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_data(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Metric::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:containerd.types.Metric)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .google.protobuf.Timestamp timestamp = 1 [(.gogoproto.nullable) = false, (.gogoproto.stdtime) = true];
  if (this->_internal_has_timestamp()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        1, _Internal::timestamp(this), target, stream);
  }

  // string id = 2;
  if (!this->_internal_id().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_id().data(), static_cast<int>(this->_internal_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "containerd.types.Metric.id");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_id(), target);
  }

  // .google.protobuf.Any data = 3;
  if (this->_internal_has_data()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        3, _Internal::data(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:containerd.types.Metric)
  return target;
}

size_t Metric::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:containerd.types.Metric)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string id = 2;
  if (!this->_internal_id().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_id());
  }

  // .google.protobuf.Timestamp timestamp = 1 [(.gogoproto.nullable) = false, (.gogoproto.stdtime) = true];
  if (this->_internal_has_timestamp()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *timestamp_);
  }

  // .google.protobuf.Any data = 3;
  if (this->_internal_has_data()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *data_);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Metric::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Metric::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Metric::GetClassData() const { return &_class_data_; }

void Metric::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<Metric *>(to)->MergeFrom(
      static_cast<const Metric &>(from));
}


void Metric::MergeFrom(const Metric& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:containerd.types.Metric)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_id().empty()) {
    _internal_set_id(from._internal_id());
  }
  if (from._internal_has_timestamp()) {
    _internal_mutable_timestamp()->PROTOBUF_NAMESPACE_ID::Timestamp::MergeFrom(from._internal_timestamp());
  }
  if (from._internal_has_data()) {
    _internal_mutable_data()->PROTOBUF_NAMESPACE_ID::Any::MergeFrom(from._internal_data());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Metric::CopyFrom(const Metric& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:containerd.types.Metric)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Metric::IsInitialized() const {
  return true;
}

void Metric::InternalSwap(Metric* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &id_, GetArenaForAllocation(),
      &other->id_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Metric, data_)
      + sizeof(Metric::data_)
      - PROTOBUF_FIELD_OFFSET(Metric, timestamp_)>(
          reinterpret_cast<char*>(&timestamp_),
          reinterpret_cast<char*>(&other->timestamp_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Metric::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_metrics_2eproto_getter, &descriptor_table_metrics_2eproto_once,
      file_level_metadata_metrics_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace types
}  // namespace containerd
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::containerd::types::Metric* Arena::CreateMaybeMessage< ::containerd::types::Metric >(Arena* arena) {
  return Arena::CreateMessageInternal< ::containerd::types::Metric >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
