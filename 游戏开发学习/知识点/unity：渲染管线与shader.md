**参考： [OpenGL图形渲染管线](https://blog.csdn.net/u012169524/article/details/113344328#:~:text=%E6%9C%AC%E6%96%87%E6%B7%B1%E5%85%A5%E6%8E%A2%E8%AE%A8OpenGL%E7%9A%84%E5%9B%BE%E5%BD%A2%E6%B8%B2%E6%9F%93%E7%AE%A1%E7%BA%BF%EF%BC%8C%E4%BB%8E%E9%A1%B6%E7%82%B9%E6%95%B0%E6%8D%AE%E5%88%B0%E5%B1%8F%E5%B9%95%E5%83%8F%E7%B4%A0%E7%9A%84%E7%9D%80%E8%89%B2%E8%BF%87%E7%A8%8B%E3%80%82%E6%B8%B2%E6%9F%93%E7%AE%A1%E7%BA%BF%E5%8C%85%E6%8B%AC%E9%A1%B6%E7%82%B9%E7%9D%80%E8%89%B2%E5%99%A8%E7%9A%84%E4%B8%96%E7%95%8C%E5%9D%90%E6%A0%87%E5%8F%98%E6%8D%A2%E3%80%81%E8%A3%81%E5%89%AA%E5%9D%90%E6%A0%87%E3%80%81%E5%85%89%E6%A0%85%E5%8C%96%E5%92%8C%E7%89%87%E6%AE%B5%E7%9D%80%E8%89%B2%E5%99%A8%E7%9A%84%E5%85%89%E7%85%A7%E8%AE%A1%E7%AE%97%E7%AD%89%E5%85%B3%E9%94%AE%E6%AD%A5%E9%AA%A4%E3%80%82) [渲染管线中的坐标变换](https://zhuanlan.zhihu.com/p/99401382) [OpenGL坐标系统 ](https://www.cnblogs.com/tandier/p/8110977.html) [光栅化的深入理解](https://blog.csdn.net/u010356727/article/details/50594401)**
[[unity：shader中的顶点信息详解]]


---
Shader 是一种用于在 GPU 上处理图形的程序，主要用于实现各种视觉效果。它通常由以下几个部分组成：
### 组成部分

1. **顶点着色器（Vertex Shader）**：
   - 处理每个顶点的数据，负责将顶点从模型空间转换到裁剪空间，计算顶点的变换和光照。

2. **片段着色器（Fragment Shader）**：
   - 处理每个片段（像素）的颜色和其他属性。它决定了最终渲染到屏幕上的颜色，执行纹理采样和光照计算。

3. **几何着色器（Geometry Shader）**（可选）：
   - 处理图元（如点、线和三角形），可以生成新的顶点或改变图元的形状。

4. **计算着色器（Compute Shader）**（可选）：
   - 用于执行通用计算，不一定与图形渲染直接相关。

### 原理

- **渲染管线**：Shader 是图形渲染管线的一部分，当图形被渲染时，数据会经过顶点着色器、几何着色器（如果存在）和片段着色器，最终生成图像。
- **并行处理**：GPU 可以同时处理大量数据，Shader 能够充分利用这一特性，以实现高效的图形计算。
### 用法

1. **编写 Shader**：
   - 使用 GLSL、HLSL 或 Cg 等语言编写 Shader 脚本，定义着色器的行为。
2. **在引擎中使用**：
   - 在游戏引擎（如 Unity、Unreal）中，创建材质并将 Shader 关联到材质上，以便在物体上应用。
3. **调整参数**：
   - 在应用程序中可以通过脚本动态调整 Shader 的参数，如颜色、纹理、光照等，以实现动态效果。

### 为什么要使用 Shader

1. **视觉效果**：
   - Shader 允许开发者实现复杂的视觉效果，如光照、阴影、反射、透明度、后期处理等，提升游戏画面的表现力。
2. **性能优化**：
   - 将计算转移到 GPU 进行并行处理，能够显著提高渲染性能，尤其是在处理大量顶点或复杂材质时。
3. **灵活性和可扩展性**：
   - 开发者可以根据需求自定义 Shader，以满足特定的视觉效果或性能要求，而不必依赖预设的材质.
4. **创新性**：
   - 使用 Shader 开发新颖的视觉效果，可以增强游戏的艺术风格，吸引玩家的注意力。

### 示例 Shader 代码

```csharp
Shader "Custom/SimpleColorShader"
{
    Properties
    {
        // 声明一个颜色属性，供用户在材质中修改
        _Color("Color", Color) = (1, 1, 1, 1) // 默认为白色
    }

    SubShader
    {
        Tags { "RenderType"="Opaque" } // 设置渲染类型为不透明

        LOD 100 // Level of Detail，设置 Shader 的细节级别

        Pass
        {
            // 设置顶点和片段着色器
            CGPROGRAM
            #pragma vertex vert // 指定顶点着色器
            #pragma fragment frag // 指定片段着色器

            // 需要包含的 Unity 头文件
            #include "UnityCG.cginc"

            // 定义颜色属性
            fixed4 _Color; // 定义为固定精度的四维向量（RGBA）

            // 定义结构体，用于传递顶点信息
            struct appdata_t
            {
                float4 vertex : POSITION; // 顶点位置
                float3 normal : NORMAL;   // 顶点法线
            };

            // 定义结构体，用于传递片段信息
            struct v2f
            {
                float4 pos : SV_POSITION; // 经过变换的顶点位置
                fixed4 color : COLOR;     // 传递的颜色
            };

            // 顶点着色器
            v2f vert(appdata_t v)
            {
                v2f o; // 输出结构体
                o.pos = UnityObjectToClipPos(v.vertex); // 将顶点转换到裁剪空间
                o.color = _Color; // 将颜色传递到片段着色器
                return o; // 返回输出
            }

            // 片段着色器
            fixed4 frag(v2f i) : SV_Target
            {
                return i.color; // 返回颜色
            }
            ENDCG
        }
    }
    FallBack "Diffuse" // 如果 Shader 不支持，使用默认的漫反射 Shader
}
```

#### 详细注释解析

1. **Shader 声明**：
   ```csharp
   Shader "Custom/SimpleColorShader"
   ```
   - 定义一个新的 Shader，名称为 "Custom/SimpleColorShader"。

2. **Properties**：
   ```csharp
   Properties
   {
       _Color("Color", Color) = (1, 1, 1, 1)
   }
   ```
   - 在材质编辑器中声明一个颜色属性 `_Color`，用户可以修改。默认值是白色 `(1, 1, 1, 1)`。

3. **SubShader**：
   ```csharp
   SubShader
   {
       Tags { "RenderType"="Opaque" }
       LOD 100
   ```
   - 定义一个 SubShader，包含渲染类型和细节级别。

4. **Pass**：
   ```csharp
   Pass
   {
       CGPROGRAM
       ...
       ENDCG
   }
   ```
   - 开始定义一个 Pass，内部使用 CGPROGRAM 和 ENDCG 包裹着色器代码。

5. **顶点和片段着色器**：
   ```csharp
   #pragma vertex vert
   #pragma fragment frag
   ```
   - 指定使用的顶点着色器和片段着色器函数。

6. **数据结构**：
   ```csharp
   struct appdata_t
   {
       float4 vertex : POSITION;
       float3 normal : NORMAL;
   };
   ```
   - 定义一个输入结构体 `appdata_t`，包含顶点位置和法线。

   ```csharp
   struct v2f
   {
       float4 pos : SV_POSITION;
       fixed4 color : COLOR;
   };
   ```
   - 定义一个输出结构体 `v2f`，用于传递变换后的顶点位置和颜色到片段着色器。

7. **顶点着色器**：
   ```csharp
   v2f vert(appdata_t v)
   {
       v2f o;
       o.pos = UnityObjectToClipPos(v.vertex);
       o.color = _Color;
       return o;
   }
   ```
   - 将输入的顶点位置转换到裁剪空间，并将颜色传递给片段着色器。

8. **片段着色器**：
   ```csharp
   fixed4 frag(v2f i) : SV_Target
   {
       return i.color;
   }
   ```
   - 返回颜色作为最终输出。

9. **FallBack**：
   ```csharp
   FallBack "Diffuse"
   ```
   - 如果当前 Shader 不支持，使用默认的漫反射 Shader 作为备选。

#### 如何使用

1. **创建 Shader 文件**：
   - 在 Unity 中右键单击项目视图，选择 "Create" > "Shader" > "Unlit Shader"，然后将上述代码复制到新创建的 Shader 文件中。

2. **创建材质**：
   - 右键单击项目视图，选择 "Create" > "Material"，然后将 Shader 应用到材质上。

3. **应用材质**：
   - 将材质拖拽到场景中的物体上，修改材质的颜色属性以查看效果。

### 总结

Shader 是现代图形编程中不可或缺的一部分，通过顶点着色器、片段着色器等组件，Shader 能够高效地实现复杂的视觉效果和优化渲染性能。它为开发者提供了灵活性和创新的空间，是提升游戏画面质量的重要工具。


### 深入：顶点数据准备
在 Unity 中，顶点数据的发送到顶点着色器以及如何筛选和管理这些顶点的过程主要包括以下几个步骤：

#### 1. 顶点数据的准备

- **Mesh 组件**：首先，模型的顶点数据是通过 Unity 的 `Mesh` 组件定义的。`Mesh` 包含了顶点位置、法线、纹理坐标等信息。
- **数据结构**：在 Unity 中，顶点数据通常以数组的形式存储，例如 `Vector3[] vertices` 用于顶点位置。

#### 2. 顶点数据的传输

- **材质和 Shader**：当你为一个物体应用材质时，Unity 会根据该材质的 Shader 将顶点数据传递给顶点着色器。
- **Shader 结构**：在 Shader 中，使用特定的输入结构体（如 `appdata_t`）来接收顶点数据。

#### 3. 从模型中筛选顶点

- **网格数据**：Unity 在加载模型时，解析网格数据（如 FBX 或 OBJ 文件），并将顶点信息存储到 `Mesh` 中。
- **绘制调用**：在进行绘制时，Unity 会根据模型的 `Mesh` 数据来筛选需要传送的顶点。通过 `Graphics.DrawMesh` 或其他 API，Unity 将当前可见的顶点数据发送到 GPU。

#### 4. 判断有多少顶点

- **顶点计数**：可以通过 `Mesh.vertexCount` 获取模型中的顶点数量。这个值在模型加载后就被确定，不会随渲染过程改变。
- **索引缓冲区**：如果使用索引缓冲区，顶点数量会根据索引的数量来确定，但仍然依赖于 `Mesh.vertexCount` 来获取实际顶点数据。

#### 示例

```csharp
Mesh mesh = GetComponent<MeshFilter>().mesh;
int vertexCount = mesh.vertexCount; // 获取顶点数量

// 在 Shader 中使用的输入结构体
struct appdata_t
{
    float4 vertex : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};
```

#### 总结

1. **顶点数据** 由 `Mesh` 组件管理，并通过 Shader 传递到顶点着色器。
2. **顶点筛选** 在加载模型时完成，Unity 会解析并存储顶点信息。
3. **顶点数量** 可以通过 `Mesh.vertexCount` 获取，确保在渲染过程中能正确处理。


### 深入：片元
#### 什么是片元

**片元（Fragment）** 是计算机图形学中的一个概念，表示在光栅化阶段生成的每个像素的候选数据。片元不仅包含颜色信息，还包含其他用于光照和纹理映射等计算的属性。每个片元可以被视为一个潜在的像素，最终会被写入到帧缓冲区。

#### 片元包含的信息

一个片元通常包含以下信息：

1. **颜色**：最终显示在屏幕上的颜色值。
2. **深度**：用于深度测试的信息，以确定哪个片元应该被绘制。
3. **纹理坐标**：用于从纹理中获取颜色的坐标。
4. **法线**：用于光照计算的表面法线。
5. **其他属性**：如光照强度、材质属性等。

#### 示例片段着色器代码

下面是一个简单的片段着色器的例子，使用 HLSL 语法：

```hlsl
Shader "Custom/SimpleFragmentShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Color ("Color", Color) = (1,1,1,1)
    }
    
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 200

        Pass
        {
            CGPROGRAM
            #pragma fragment frag

            struct v2f
            {
                float2 uv : TEXCOORD0; // 纹理坐标
                float4 color : COLOR;   // 顶点颜色
            };

            // 片段着色器
            fixed4 frag(v2f i) : SV_Target
            {
                // 从纹理中获取颜色
                fixed4 texColor = tex2D(_MainTex, i.uv);
                
                // 将纹理颜色与传入的颜色相乘
                fixed4 finalColor = texColor * i.color;
                
                return finalColor; // 返回最终颜色
            }
            ENDCG
        }
    }
}
```

#### 代码解释

1. **Shader 定义**：
   - `Shader "Custom/SimpleFragmentShader"`：定义了一个名为 "Custom/SimpleFragmentShader" 的 Shader。

2. **Properties**：
   - `Properties` 部分定义了材质可以调整的属性，包括一个纹理和一个颜色。

3. **SubShader**：
   - `SubShader` 是 Shader 的主要部分，可以包含多个渲染通道（Pass）。在这个例子中，只有一个 `Pass`。

4. **Pass**：
   - `Pass` 定义了实际的渲染步骤。使用 `CGPROGRAM` 指定使用 Cg/HLSL 编写程序。

5. **结构体 `v2f`**：
   - `v2f` 结构体定义了从顶点着色器传递到片段着色器的数据，包括纹理坐标和颜色。

6. **片段着色器 `frag`**：
   - `fixed4 frag(v2f i) : SV_Target`：这是片段着色器的主函数，它接收来自 `v2f` 结构体的数据。
   - `fixed4 texColor = tex2D(_MainTex, i.uv);`：从纹理中获取颜色，使用传入的纹理坐标。
   - `fixed4 finalColor = texColor * i.color;`：将获取的纹理颜色与传入的颜色相乘，得到最终颜色。
   - `return finalColor;`：返回最终颜色值，以便写入到帧缓冲区。

#### 总结

- **片元** 是光栅化阶段生成的候选像素，包含颜色、深度、纹理坐标等信息。
- 示例代码展示了如何在片段着色器中处理纹理和颜色，生成最终的颜色值。
