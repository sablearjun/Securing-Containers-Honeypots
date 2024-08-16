// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: platform.proto

#include "platform.pb.h"

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
constexpr Platform::Platform(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : os_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , architecture_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , variant_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string){}
struct PlatformDefaultTypeInternal {
  constexpr PlatformDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~PlatformDefaultTypeInternal() {}
  union {
    Platform _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PlatformDefaultTypeInternal _Platform_default_instance_;
}  // namespace types
}  // namespace containerd
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_platform_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_platform_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_platform_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_platform_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::containerd::types::Platform, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::containerd::types::Platform, os_),
  PROTOBUF_FIELD_OFFSET(::containerd::types::Platform, architecture_),
  PROTOBUF_FIELD_OFFSET(::containerd::types::Platform, variant_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::containerd::types::Platform)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::containerd::types::_Platform_default_instance_),
};

const char descriptor_table_protodef_platform_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016platform.proto\022\020containerd.types\032\ngogo"
  ".proto\"E\n\010Platform\022\022\n\002os\030\001 \001(\tB\006\342\336\037\002OS\022\024"
  "\n\014architecture\030\002 \001(\t\022\017\n\007variant\030\003 \001(\tB2Z"
  "0github.com/containerd/containerd/api/ty"
  "pes;typesb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_platform_2eproto_deps[1] = {
  &::descriptor_table_gogo_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_platform_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_platform_2eproto = {
  false, false, 177, descriptor_table_protodef_platform_2eproto, "platform.proto", 
  &descriptor_table_platform_2eproto_once, descriptor_table_platform_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_platform_2eproto::offsets,
  file_level_metadata_platform_2eproto, file_level_enum_descriptors_platform_2eproto, file_level_service_descriptors_platform_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_platform_2eproto_getter() {
  return &descriptor_table_platform_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_platform_2eproto(&descriptor_table_platform_2eproto);
namespace containerd {
namespace types {

// ===================================================================

class Platform::_Internal {
 public:
};

Platform::Platform(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:containerd.types.Platform)
}
Platform::Platform(const Platform& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  os_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_os().empty()) {
    os_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_os(), 
      GetArenaForAllocation());
  }
  architecture_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_architecture().empty()) {
    architecture_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_architecture(), 
      GetArenaForAllocation());
  }
  variant_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_variant().empty()) {
    variant_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_variant(), 
      GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:containerd.types.Platform)
}

inline void Platform::SharedCtor() {
os_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
architecture_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
variant_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

Platform::~Platform() {
  // @@protoc_insertion_point(destructor:containerd.types.Platform)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void Platform::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  os_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  architecture_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  variant_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void Platform::ArenaDtor(void* object) {
  Platform* _this = reinterpret_cast< Platform* >(object);
  (void)_this;
}
void Platform::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Platform::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void Platform::Clear() {
// @@protoc_insertion_point(message_clear_start:containerd.types.Platform)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  os_.ClearToEmpty();
  architecture_.ClearToEmpty();
  variant_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Platform::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string os = 1 [(.gogoproto.customname) = "OS"];
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_os();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "containerd.types.Platform.os"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string architecture = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_architecture();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "containerd.types.Platform.architecture"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // string variant = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_variant();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "containerd.types.Platform.variant"));
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

::PROTOBUF_NAMESPACE_ID::uint8* Platform::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:containerd.types.Platform)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string os = 1 [(.gogoproto.customname) = "OS"];
  if (!this->_internal_os().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_os().data(), static_cast<int>(this->_internal_os().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "containerd.types.Platform.os");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_os(), target);
  }

  // string architecture = 2;
  if (!this->_internal_architecture().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_architecture().data(), static_cast<int>(this->_internal_architecture().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "containerd.types.Platform.architecture");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_architecture(), target);
  }

  // string variant = 3;
  if (!this->_internal_variant().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_variant().data(), static_cast<int>(this->_internal_variant().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "containerd.types.Platform.variant");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_variant(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:containerd.types.Platform)
  return target;
}

size_t Platform::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:containerd.types.Platform)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string os = 1 [(.gogoproto.customname) = "OS"];
  if (!this->_internal_os().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_os());
  }

  // string architecture = 2;
  if (!this->_internal_architecture().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_architecture());
  }

  // string variant = 3;
  if (!this->_internal_variant().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_variant());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Platform::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    Platform::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Platform::GetClassData() const { return &_class_data_; }

void Platform::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<Platform *>(to)->MergeFrom(
      static_cast<const Platform &>(from));
}


void Platform::MergeFrom(const Platform& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:containerd.types.Platform)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_os().empty()) {
    _internal_set_os(from._internal_os());
  }
  if (!from._internal_architecture().empty()) {
    _internal_set_architecture(from._internal_architecture());
  }
  if (!from._internal_variant().empty()) {
    _internal_set_variant(from._internal_variant());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Platform::CopyFrom(const Platform& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:containerd.types.Platform)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Platform::IsInitialized() const {
  return true;
}

void Platform::InternalSwap(Platform* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &os_, GetArenaForAllocation(),
      &other->os_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &architecture_, GetArenaForAllocation(),
      &other->architecture_, other->GetArenaForAllocation()
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &variant_, GetArenaForAllocation(),
      &other->variant_, other->GetArenaForAllocation()
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata Platform::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_platform_2eproto_getter, &descriptor_table_platform_2eproto_once,
      file_level_metadata_platform_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace types
}  // namespace containerd
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::containerd::types::Platform* Arena::CreateMaybeMessage< ::containerd::types::Platform >(Arena* arena) {
  return Arena::CreateMessageInternal< ::containerd::types::Platform >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
