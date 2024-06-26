#pragma once
#include "Core/Base.h"
#include "Common.h"

namespace graphic {
enum class PolygonMode
{
    Fill, 
    Line
};

enum class FrontFaceType
{
    CLOCKWISE ,
    COUNTER_CLOCKWISE
};

enum class TopologyType {
    TRANGLE_LIST,
    LINE_LIST,
    POINT_LIST,
    MAX_ENUM
};

enum class CullMode{
    NONE,
    FRONT,
    BACK,
};

enum class BlendOperation {
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MINIMUM,
    MAXIMUM, // Yes, this one is an actual operator.
    MAX_ENUM
};

enum class BlendFactor {
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE,
    SRC1_COLOR,
    ONE_MINUS_SRC1_COLOR,
    SRC1_ALPHA,
    ONE_MINUS_SRC1_ALPHA,
    MAX_ENUM
};

struct VertexBindInfo
{
	enum InputRate
	{
		INPUT_RATE_VERTEX,
		INPUT_RATE_INSTANCE
	};

	u32 binding;
	u32 stride;
	InputRate inputRate;
};

struct VertexAttribInfo
{
	enum AttribFormat
	{
		ATTRIB_FORMAT_VEC2,
		ATTRIB_FORMAT_VEC3,
		ATTRIB_FORMAT_VEC4,
	};

	u32 binding;
	u32 location;
	u32 offset;
	AttribFormat format;
};

struct PipelineColorBlendState {
    bool enable_logic_op = false;
    LogicOperation logic_op = LogicOperation::CLEAR;

    struct Attachment {
        bool enable_blend = false;
        BlendFactor srcColorBlendFactor = BlendFactor::ZERO;
        BlendFactor dstColorBlendFactor = BlendFactor::ZERO;
        BlendOperation colorBlendOp = BlendOperation::ADD;
        BlendFactor srcAlphaBlendFactor = BlendFactor::ZERO;
        BlendFactor dstAlphaBlendFactor = BlendFactor::ZERO;
        BlendOperation alphaBlendOp = BlendOperation::ADD;
        bool writeR = true;
        bool writeG = true;
        bool writeB = true;
        bool writeA = true;
    };

    static PipelineColorBlendState create_disabled(int p_attachments = 1) {
        PipelineColorBlendState bs;
        for (int i = 0; i < p_attachments; i++) {
            bs.attachments.push_back(Attachment());
        }
        return bs;
    }

    static PipelineColorBlendState create_blend(int p_attachments = 1) {
        PipelineColorBlendState bs;
        for (int i = 0; i < p_attachments; i++) {
            Attachment ba;
            ba.enable_blend = true;
            ba.srcColorBlendFactor = BlendFactor::SRC_ALPHA;
            ba.dstColorBlendFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
            ba.srcAlphaBlendFactor = BlendFactor::SRC_ALPHA;
            ba.dstColorBlendFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;

            bs.attachments.push_back(ba);
        }
        return bs;
    }

    std::vector<Attachment> attachments; // One per render target texture.
};

struct RasterizationState {
    CullMode cullMode = CullMode::NONE;
    FrontFaceType frontFaceType = FrontFaceType::COUNTER_CLOCKWISE;
    PolygonMode polygonMode = PolygonMode::Fill;
    bool enableDepthClamp = false;
    bool enableAntialiasedLine = false;
    SampleCount SampleCount = SampleCount::SAMPLES_1;
    float lineWidth = 1.f;
};

struct PipelineDepthStencilState {
    bool enableDepthTest = false;
    bool enableDepthWrite = false;
    CompareOperation depthCompareOp = CompareOperation::LESS_OR_EQUAL;
    bool enableDepthRange = false;
    float depthRangeMin = 0;
    float depthRangeMax = 0;
    bool enableStencil = false;
};

struct GraphicPipeLineDesc {
    Ref<Shader> vertShader;
    Ref<Shader> fragShader;
    PipelineColorBlendState blendState = {};
    RasterizationState rasterState = {};
    PipelineDepthStencilState depthStencilState = {};
    std::vector<VertexBindInfo> vertexBindInfos;
    std::vector<VertexAttribInfo>vertexAttribInfos;
    TopologyType topologyType = TopologyType::TRANGLE_LIST;
    // For dynamic rendering
    std::vector<DataFormat> colorAttachmentFormats;
    DataFormat depthAttachmentFormat = DataFormat::UNDEFINED; 
};


enum class PipeLineType {
    GRAPHIC,
    COMPUTE
};

class PipeLine : public GpuResource{
public:
    virtual ~PipeLine() = default;
protected:
    PipeLine(PipeLineType type) : type_(type) {};
    Ref<Shader> vertShader_;
    Ref<Shader> fragShder_;
    Ref<Shader> computeShader_;
    PipeLineType type_;
};

}