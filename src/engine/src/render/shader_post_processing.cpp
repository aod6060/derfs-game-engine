#include "../sys.hpp"



namespace render {
    namespace shader {
        namespace postprocess {
            CopyPostProcessShader copyShader;
            GaussianBlurPostProcessShader gaussianBlurShader;
            CombinePostProcessShader combineShader;
            ThresholdPostProcessShader thresholdShader;
            ModifiedEdgeDetectionPostProcessShader modifiedEdgeDetectionShader;
            InvertPostProcessShader invertShader;
            DesaturatePostProcessShader desatShader;

            void init() {
                copyShader.setFragmentShaderPath("data/shaders/post_pass/copy.fs.glsl");
                copyShader.init();
                gaussianBlurShader.setFragmentShaderPath("data/shaders/post_pass/gaussian_blur.fs.glsl");
                gaussianBlurShader.init();
                combineShader.setFragmentShaderPath("data/shaders/post_pass/combine.fs.glsl");
                combineShader.init();
                thresholdShader.setFragmentShaderPath("data/shaders/post_pass/threshold.fs.glsl");
                thresholdShader.init();
                modifiedEdgeDetectionShader.setFragmentShaderPath("data/shaders/post_pass/modified_edge_detection.glsl");
                modifiedEdgeDetectionShader.init();
                invertShader.setFragmentShaderPath("data/shaders/post_pass/invert.fs.glsl");
                invertShader.init();
                desatShader.setFragmentShaderPath("data/shaders/post_pass/desaturate.fs.glsl");
                desatShader.init();
            }

            void release() {
                desatShader.release();
                invertShader.release();
                modifiedEdgeDetectionShader.release();
                thresholdShader.release();
                combineShader.release();
                gaussianBlurShader.release();
                copyShader.release();
            }

            CopyPostProcessShader* getCopyShader() {
                return &copyShader;
            }

            GaussianBlurPostProcessShader* getGaussianBlurShader() {
                return &gaussianBlurShader;
            }

            CombinePostProcessShader* getCombineShader() {
                return &combineShader;
            }

            ThresholdPostProcessShader* getThresholdShader() {
                return &thresholdShader;
            }

            ModifiedEdgeDetectionPostProcessShader* getModifiedEdgeDetectionShader() {
                return &modifiedEdgeDetectionShader;
            }

            InvertPostProcessShader* getInvertShader() {
                return &invertShader;
            }

            DesaturatePostProcessShader* getDesaturateShader() {
                return &desatShader;
            }

            // PostProcessShader
            void PostProcessShader::init() {
                this->vertexShader.init(GL_VERTEX_SHADER, "data/shaders/standard2D.vs.glsl");
                this->fragmentShader.init(GL_FRAGMENT_SHADER, this->fragmentShaderPath);
                this->program.init({&this->vertexShader, &this->fragmentShader});

                this->bind();

                // Uniform Buffer Object
                this->program.uniformBlock.createUniformBlock("Standard2DTransform", 1);

                // Attributes
                this->program.attributes.createAttribute("vertices", 0);
                this->program.attributes.createAttribute("texCoords", 1);

                this->program.attributes.bind();
                this->program.attributes.enableAttribute("vertices");
                this->program.attributes.enableAttribute("texCoords");
                this->program.attributes.unbind();

                // This function be called to help build a postprocess shader
                this->buildShader();

                this->unbind();
            }

            void PostProcessShader::release() {
                this->program.release();
                this->fragmentShader.release();
                this->vertexShader.release();
            }

            void PostProcessShader::bind() {
                this->program.bind();
            }

            void PostProcessShader::unbind() {
                this->program.unbind();
            }

            void PostProcessShader::bindVertexArray() {
                this->program.attributes.bind();
            }

            void PostProcessShader::unbindVertexArray() {
                this->program.attributes.unbind();
            }

            void PostProcessShader::verticesPointer() {
                this->program.attributes.attributePointer("vertices", 3, GL_FLOAT);
            }

            void PostProcessShader::texCoordPointer() {
                this->program.attributes.attributePointer("texCoords", 2, GL_FLOAT);
            }

            void PostProcessShader::setFragmentShaderPath(std::string path) {
                this->fragmentShaderPath = path;
            }

            // CopyPostProcessShader
            void CopyPostProcessShader::buildShader() {
                // Uniform
                this->program.uniforms.createUniform("tex0");
                this->program.uniforms.uniform1i("tex0", 0);
            }


            // GaussianBlurPostProcessShader
            void GaussianBlurPostProcessShader::buildShader() {
                this->program.uniforms.createUniform("tex0");
                this->program.uniforms.uniform1i("tex0", 0);
                this->program.uniforms.createUniform("sampleDistance");
            }

            void GaussianBlurPostProcessShader::setSampleDistance(float value) {
                this->program.uniforms.uniform1f("sampleDistance", value);
            }

            // CombinePostProcessShader
            void CombinePostProcessShader::buildShader() {
                this->program.uniforms.createUniform("op");
                this->program.uniforms.createUniform("a");
                this->program.uniforms.uniform1i("a", 0);
                this->program.uniforms.createUniform("b");
                this->program.uniforms.uniform1i("b", 1);
                this->program.uniforms.createUniform("mixValue");
            }

            void CombinePostProcessShader::setCombineOp(CombineOP op) {
                this->program.uniforms.uniform1i("op", (int)op);
            }

            // If COMBINE_OP_MIX is used it will be 
            // a Value between 0 -> 1 using the mix function
            void CombinePostProcessShader::setMixValue(float value) {
                this->program.uniforms.uniform1f("mixValue", value);
            }

            // ThresholdPostProcessShader
            void ThresholdPostProcessShader::buildShader() {
                this->program.uniforms.createUniform("tex0");
                this->program.uniforms.uniform1i("tex0", 0);
                this->program.uniforms.createUniform("minValue");
                this->program.uniforms.createUniform("maxValue");
            }

            void ThresholdPostProcessShader::setMinValue(float value) {
                this->program.uniforms.uniform1f("minValue", value);
            }

            void ThresholdPostProcessShader::setMaxValue(float value) {
                this->program.uniforms.uniform1f("maxValue", value);
            }

            // ModifiedEdgeDetectionPostProcessShader
            void ModifiedEdgeDetectionPostProcessShader::buildShader() {
                this->program.uniforms.createUniform("tex0");
                this->program.uniforms.uniform1i("tex0", 0);
                this->program.uniforms.createUniform("sampleDistance");
            }

            void ModifiedEdgeDetectionPostProcessShader::setSampleDistance(float value) {
                this->program.uniforms.uniform1f("sampleDistance", value);
            }

            // Invert Shader
            void InvertPostProcessShader::buildShader() {
                this->program.uniforms.createUniform("tex0");
                this->program.uniforms.uniform1i("tex0", 0);
            }

            // DesaturateShader
            void DesaturatePostProcessShader::buildShader() {
                this->program.uniforms.createUniform("tex0");
                this->program.uniforms.uniform1i("tex0", 0);
                this->program.uniforms.createUniform("value");
            }

            void DesaturatePostProcessShader::setValue(float value) {
                this->program.uniforms.uniform1f("value", value);
            }

        }
    }
}