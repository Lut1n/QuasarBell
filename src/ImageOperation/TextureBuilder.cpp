#include "ImageOperation/TextureBuilder.hpp"

#include "App/ImageNode.hpp"
#include "App/GeometryNode.hpp"
#include "Graphics/TgaExporter.hpp"
#include "ImageOperation/FrameRenderer.hpp"
#include "SdfOperation/VoxelExporter.hpp"

std::unique_ptr<FrameRenderer> TextureGenerator::frameRenderer;

TextureGenerator::TextureGenerator()
{
    registerImageOperation<ColorInput>();
    registerImageOperation<Construct3f>();
    registerImageOperation<Split3f>();
    registerImageOperation<Dynamics>();
    registerImageOperation<DirectionalSignal>();
    registerImageOperation<RadialSignal>();
    registerImageOperation<TimeInput>();
    registerImageOperation<HighResOutput>();
    registerImageOperation<UvInput>();
    registerImageOperation<SphericalCoord>();
    registerImageOperation<UvDistortion>();
    registerImageOperation<UvMapping>();
    registerImageOperation<BlurFilter>();
    registerImageOperation<SharpenFilter>();
    registerImageOperation<MorphoFilter>();
    registerImageOperation<BumpToNormal>();
    registerImageOperation<WhiteNoise>();
    registerImageOperation<ValueNoise>();
    registerImageOperation<GradientNoise>();
    registerImageOperation<SimplexNoise>();
    registerImageOperation<VoronoiNoise>();
    registerImageOperation<Mandelbrot>();
    registerImageOperation<ImageAdd>();
    registerImageOperation<ImageSub>();
    registerImageOperation<ImageMult>();
    registerImageOperation<ImageDiv>();
    registerImageOperation<ImageMix>();
    registerImageOperation<ImageClamp>();
    registerImageOperation<ImageDot>();
    registerImageOperation<ImageCross>();
    registerImageOperation<ImageStep>();
    registerImageOperation<ImageSin>();
    registerImageOperation<ImageCos>();
    registerImageOperation<ImageTan>();
    registerImageOperation<ImageMod>();
    registerImageOperation<ImagePow>();
    registerImageOperation<ImageSqrt>();
    registerImageOperation<ImageAbs>();
    registerImageOperation<ImageLog>();
    registerImageOperation<ImageExp>();
    registerImageOperation<ImageMin>();
    registerImageOperation<ImageMax>();
    
    registerSdfOperation<Sphere>();
    registerSdfOperation<Box>();
    registerSdfOperation<RoundBox>();
    registerSdfOperation<Torus>();
    registerSdfOperation<HexagonalPrism>();
    registerSdfOperation<TriangularPrism>();
    registerSdfOperation<Capsule>();
    registerSdfOperation<Cone>();
    registerSdfOperation<RoundedCone>();
    registerSdfOperation<Cylinder>();
    registerSdfOperation<RoundedCylinder>();
    registerSdfOperation<Octahedron>();
    registerSdfOperation<Pyramid>();
    registerSdfOperation<Union>();
    registerSdfOperation<Substraction>();
    registerSdfOperation<Intersection>();
    registerSdfOperation<SmoothUnion>();
    registerSdfOperation<SmoothSubstraction>();
    registerSdfOperation<SmoothIntersection>();
    registerSdfOperation<Transform>();
    registerSdfOperation<Repetition>();
    registerSdfOperation<Twist>();
    registerSdfOperation<Bend>();
    registerSdfOperation<Elongation>();
    registerSdfOperation<Symmetry>();
    registerSdfOperation<Displacement>();
    registerSdfOperation<Texturing>();
    registerSdfOperation<HighResSdfOutput>();
    registerSdfOperation<VoxelOutput>();
}

void TextureGenerator::prepare(BaseOperationNode* node)
{
    if (auto output = dynamic_cast<ImageOutput*>(node->getAttributes()))
    {
        int& resolution = output->textureResolution;
        resolution = (int)std::pow(2.0,(float)output->resolution);

        state.resolution = resolution;
        state.time = 0.0f;

        state.path = output->path;
        state.toExport = output->toExport;
        state.sdfMode = false;
        output->toExport = false;
    }
    else if (auto output = dynamic_cast<HighResSdfOutputData*>(node->getAttributes()))
    {
        int& resolution = output->textureResolution;
        resolution = (int)std::pow(2.0,(float)output->resolution);

        state.resolution = resolution;
        state.time = 0.0f;

        state.path = "";
        state.toExport = false;
        state.sdfMode = true;
        // output->toExport = false;
    }
    else if (auto output = dynamic_cast<VoxelOutputData*>(node->getAttributes()))
    {
        state.resolution = 1024;
        state.time = 0.0f;
        state.voxelOutput = true;
        // state.path = output->path;
        state.toExport = output->toExport;
        state.sdfMode = true;
        output->toExport = false;
    }
}

TexturePreview* TextureGenerator::createPreview(int resolution)
{
    auto it = previews.find(currentNode);
    if (it == previews.end())
    {
        auto result = std::make_unique<TexturePreview>(resolution);
        previews[currentNode] = std::move(result);
    }

    return static_cast<TexturePreview*>(previews[currentNode].get());
}

bool TextureGenerator::hasChanged()
{
    bool ret = false;
    state.recompile = false;
    state.selectedNode = nullptr;
    for(auto node : collector.nodes)
    {
        auto attributesChanged = node->getAttributes()->hasChanged();
        auto connectionChanged = node->getAttributes()->connectionChanged();
        auto selected = BaseOperationNode::isSelected(node);

        ret = attributesChanged || connectionChanged || ret;
        if (connectionChanged)
            state.recompile = true;
        if (selected)
            state.selectedNode = node;
    }
    return ret;
}

void TextureGenerator::prepareInputInfos()
{
    infos.clear();

    for(auto node : collector.nodes)
        infos[node] = TextureOperationInfo();

    for(auto node : collector.nodes)
    {
        auto& info = infos[node];
        std::unordered_map<size_t, std::unique_ptr<AbstractImageOperation>>::iterator op;
        if (dynamic_cast<ImageNode*>(node))
        {
            info.isSdf = false;
            op = imageOperations.find(node->nodeTypeId());
        }
        else if (dynamic_cast<GeometryNode*>(node))
        {
            info.isSdf = true;
            op = sdfOperations.find(node->nodeTypeId());
        }

        info.operation = op->second.get();
        info.attributes = node->getAttributes();
        for(int i=0; i<node->getSourceCount(); ++i)
        {
            auto source = static_cast<BaseOperationNode*>(node->getSourceNode(i));
            if (source)
                info.inputs.push_back( &infos[source] );
            else
                info.inputs.push_back(nullptr);
        }
    }
}

void TextureGenerator::compute(BaseOperationNode* outputNode)
{
    state = State();
    collector.nodes.clear();
    outputNode->accept(collector);
    bool changed = hasChanged();

    if (changed || dynamic_cast<VoxelOutputData*>(outputNode->getAttributes()))
        prepare(outputNode);

    if (state.recompile)
    {
        prepareInputInfos();
        CleanPreviews cleaner;
        outputNode->accept(cleaner);
        previews.clear();
    }

    if (state.voxelOutput)
    {
        computeVoxels(outputNode);
        state.toExport = false;
    }
    
    if (!changed)
    {
        if (state.selectedNode)
            computePreviews(state.selectedNode);
        return;
    }

    if (state.toExport)
        generateTexture(outputNode);

    if (changed)
    {
        for(auto node : collector.nodes)
            computePreviews(node);
    }
}

void TextureGenerator::computeResult(BaseOperationNode* node, TexturePreview* preview)
{
    auto& info = infos[node];

    TextureOperationResult result;
    result.lastAttributes = info.attributes;
    
    auto& visitor = result.visitor;
    visitor.mainFrame.type = dynamic_cast<GeometryNode*>(node) ? qb::GlslFrame::Type::Sdf : qb::GlslFrame::Type::Texture;
    visitor.mainFrame.resolution = preview->resolution;
    visitor.setCurrentOperation(nullptr);
    info.operation->apply(result, info.attributes, info.inputs);
    visitor.unsetCurrentOperation();

    if(!frameRenderer)
        frameRenderer = std::make_unique<FrameRenderer>();

    frameRenderer->render(preview, &visitor.mainFrame);
}

void TextureGenerator::computePreviews(BaseOperationNode* node)
{
    bool isOutput = dynamic_cast<ImageOutput*>(node->getAttributes()) || dynamic_cast<HighResSdfOutputData*>(node->getAttributes());

    currentNode = node;
    auto texturePreview = createPreview( isOutput ? state.resolution : state.defaultResolution );

    computeResult(node, texturePreview);

    node->setPreview(previews[node].get());
    node->getAttributes()->validate();
}

void TextureGenerator::generateTexture(BaseOperationNode* node)
{
    currentNode = node;
    auto texturePreview = std::make_unique<TexturePreview>(state.resolution);

    computeResult(node, texturePreview.get());
    qb::ImageData image = RenderInterface::downloadTargetImage((unsigned)texturePreview->glTextureId);
    qb::exportTGA(state.path, image);
}

//--------------------------------------------------------------
void scanZ(BaseOperationNode* node, VoxelOutputData* attributes, TexturePreview* result)
{
    auto& scanningWork = attributes->scanningWork;
    auto& work = scanningWork.getWork<qb::ScanningWork>();

    qb::ImageData slice = RenderInterface::downloadTargetImage(result->glTextureId);

    work.result.forEach([&](size_t x, size_t y){
        unsigned s = slice.sample(x * work.nStep, y * work.nStep);
        work.result.set(x,work.lastRendered,(work.count-1)-y, s);
    });
    
    scanningWork.progress = (float)work.lastRendered / work.count;
    if (scanningWork.progress >= 1.0)
        scanningWork.completed = true;
}

void TextureGenerator::computeVoxels(BaseOperationNode* node)
{
    auto voxelOutput = dynamic_cast<VoxelOutputData*>(node->getAttributes());
    auto& scanningWork = voxelOutput->scanningWork;
    auto& optimizationWork = voxelOutput->optimizationWork;
    auto& indexingWork = voxelOutput->indexingWork;
    auto& exportationWork = voxelOutput->exportationWork;
    auto voxelSize = voxelOutput->voxelSize;

    if (scanningWork.initialized)
    {
        auto& work = scanningWork.getWork<qb::ScanningWork>();

        if (exportationWork.completed)
            voxelOutput->resetWorks();
        else if (indexingWork.completed)
            qb::exportVoxel("./voxelOutput.vox", qb::getOptimizedVoxel(optimizationWork), exportationWork);
        else if (optimizationWork.completed)
             qb::colorIndexing(qb::getOptimizedVoxel(optimizationWork), indexingWork);
        else if (scanningWork.completed)
            qb::optimizeVoxels(work.result, optimizationWork);
        else
        {
            computePreviews(node);
            work.lastRendered++;
            work.zPlan += voxelSize;
            scanZ(node, voxelOutput, dynamic_cast<TexturePreview*>(previews[node].get()));
        }
    }
    else if (state.toExport)
    {
        scanningWork.initialize<qb::ScanningWork>();
        auto& work = scanningWork.getWork<qb::ScanningWork>();
        work.nStep = voxelSize * 0.25f;
        work.count = (size_t)(1.0f / work.nStep);
        work.zPlan = -2.0f + voxelSize * 0.5f;
        work.result.resize(work.count, work.count, work.count);
    }
}