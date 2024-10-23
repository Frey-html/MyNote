
### 1. 位置（Position）

#### 作用
位置信息定义了顶点在三维空间中的坐标，通常是三维向量（x, y, z）。

#### 使用场景
用于确定几何体的位置，是绘制任何三维形状的基础。

#### 示例 Shader
在 Vertex Shader 中处理位置信息，通常会将其从模型空间转换到裁剪空间。

```hlsl
Shader "Custom/PositionShader" {
    Properties {
        _Color("Color", Color) = (1, 1, 1, 1)
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            
            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            fixed4 _Color;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex); // 从模型空间转换到裁剪空间
                o.uv = v.uv;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                fixed4 col = tex2D(_MainTex, i.uv) * _Color;
                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 2. 法线（Normal）

#### 作用
法线表示顶点处的表面方向，用于光照计算。

#### 使用场景
计算光照效果，包括漫反射、镜面反射等。

#### 示例 Shader
使用法线进行光照计算。

```hlsl
Shader "Custom/LitShader" {
    Properties {
        _Color("Color", Color) = (1, 1, 1, 1)
        _MainTex("Base (RGB)", 2D) = "white" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
                float3 worldNormal : TEXCOORD1; // 世界空间的法线
            };

            fixed4 _Color;
            sampler2D _MainTex;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;

                // 将模型空间法线转换为世界空间法线
                o.worldNormal = mul((float3x3)UNITY_MATRIX_IT_MV, v.normal);

                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                // 使用法线进行光照计算
                float3 lightDir = normalize(_WorldSpaceLightPos0.xyz);
                float diffuse = max(dot(i.worldNormal, lightDir), 0.0);

                fixed4 col = tex2D(_MainTex, i.uv) * _Color;
                col.rgb *= diffuse;

                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 3. 纹理坐标（Texture Coordinates）

#### 作用
定义了顶点在纹理图上的二维坐标 (u, v)，用来贴图。

#### 使用场景
当渲染带有纹理的物体时，纹理坐标用于确定纹理图上哪些像素映射到三维物体的表面上。

#### 示例 Shader
使用纹理坐标贴图。

```hlsl
Shader "Custom/TextureShader" {
    Properties {
        _MainTex("Base (RGB)", 2D) = "white" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                fixed4 col = tex2D(_MainTex, i.uv);
                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 4. 颜色（Color）

#### 作用
直接赋予顶点颜色值。

#### 使用场景
当不需要复杂的光照计算或者在某些情况下直接控制顶点颜色，例如在调试阶段或一些特定的艺术风格中。

#### 示例 Shader
直接使用顶点颜色。

```hlsl
Shader "Custom/VertexColorShader" {
    Properties {
        _MainTex("Base (RGB)", 2D) = "white" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float4 color : COLOR;
            };

            struct v2f {
                float4 vertex : SV_POSITION;
                fixed4 color : COLOR;
            };

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.color = v.color;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                return i.color;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 5. 切线（Tangent）

#### 作用
表示一个局部坐标系中的一个轴向，通常与顶点的法线一起使用。

#### 使用场景
用于法线贴图的正确解码。

#### 示例 Shader
使用切线空间法线贴图。

```hlsl
Shader "Custom/NormalMapShader" {
    Properties {
        _MainTex("Base (RGB)", 2D) = "white" {}
        _BumpMap("Normal Map", 2D) = "bump" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
                float3 tangent : TANGENT;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
                float3 worldTangent : TEXCOORD1; // 世界空间的切线
            };

            sampler2D _MainTex;
            sampler2D _BumpMap;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;

                // 将模型空间切线转换为世界空间切线
                o.worldTangent = mul((float3x3)UNITY_MATRIX_IT_MV, v.tangent);

                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                // 从法线贴图中获取切线空间的法线
                float3 bump = tex2D(_BumpMap, i.uv).xyz * 2.0 - 1.0;

                // 计算世界空间中的法线
                float3 worldNormal = normalize(mul((float3x3)UNITY_MATRIX_IT_MV, bump));

                // 应用光照计算
                float3 lightDir = normalize(_WorldSpaceLightPos0.xyz);
                float diffuse = max(dot(worldNormal, lightDir), 0.0);

                fixed4 col = tex2D(_MainTex, i.uv);
                col.rgb *= diffuse;

                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 6. 色差（Binormal）

#### 作用
与切线共同定义切线空间的另一个轴向。

#### 使用场景
与切线一起形成一个正交坐标系，用于解码法线贴图中的信息。

#### 示例 Shader
在法线贴图解码时，使用切线、法线和色差共同构建切线空间。

```hlsl
Shader "Custom/BinormalShader" {
    Properties {
        _MainTex("Base (RGB)", 2D) = "white" {}
        _BumpMap("Normal Map", 2D) = "bump" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
                float3 tangent : TANGENT;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
                float3 worldTangent : TEXCOORD1; // 世界空间的切线
                float3 worldBitangent : TEXCOORD2; // 世界空间的色差
            };

            sampler2D _MainTex;
            sampler2D _BumpMap;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;

                // 将模型空间切线转换为世界空间切线
                o.worldTangent = mul((float3x3)UNITY_MATRIX_IT_MV, v.tangent);
                o.worldBitangent = mul((float3x3)UNITY_MATRIX_IT_MV, cross(v.normal, v.tangent) * v.tangent.w);

                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                // 从法线贴图中获取切线空间的法线
                float3 bump = tex2D(_BumpMap, i.uv).xyz * 2.0 - 1.0;

                // 计算世界空间中的法线
                float3 worldNormal = normalize(float3(dot(bump, i.worldTangent), dot(bump, i.worldBitangent), dot(bump, i.worldNormal)));

                // 应用光照计算
                float3 lightDir = normalize(_WorldSpaceLightPos0.xyz);
                float diffuse = max(dot(worldNormal, lightDir), 0.0);

                fixed4 col = tex2D(_MainTex, i.uv);
                col.rgb *= diffuse;

                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 7. 骨骼权重（Bone Weights）

#### 作用
骨骼权重表示顶点受多个骨骼的影响程度。

#### 使用场景
在制作具有复杂动作的角色时，通常会使用骨骼动画技术。

#### 示例 Shader
使用骨骼权重进行皮肤绑定。

```hlsl
Shader "Custom/SkinningShader" {
    Properties {
        _MainTex("Base (RGB)", 2D) = "white" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
                float4 boneWeights : BLENDWEIGHT;
                uint4 boneIndices : BONEINDICES;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            uniform float4x4 _BoneMatrices[8]; // 假设有最多8个骨骼

            v2f vert (appdata v) {
                v2f o;
                o.uv = v.uv;

                // 计算加权后的顶点位置
                float4 finalPos = float4(0, 0, 0, 1);
                for (int i = 0; i < 4; ++i) {
                    int index = v.boneIndices[i];
                    if (index < 8) {
                        finalPos += mul(_BoneMatrices[index], float4(v.vertex.xyz, 1.0)) * v.boneWeights[i];
                    }
                }

                o.vertex = UnityObjectToClipPos(finalPos);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                fixed4 col = tex2D(_MainTex, i.uv);
                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 8. 骨骼索引（Bone Indices）

#### 作用
骨骼索引是指与顶点关联的骨骼 ID。

#### 使用场景
配合骨骼权重使用，骨骼索引告诉渲染器哪些骨骼影响了该顶点。

#### 示例 Shader
使用骨骼索引进行皮肤绑定。

```hlsl
Shader "Custom/SkinningWithIndicesShader" {
    Properties {
        _MainTex("Base (RGB)", 2D) = "white" {}
    }

    SubShader {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
                float4 boneWeights : BLENDWEIGHT;
                uint4 boneIndices : BONEINDICES;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            uniform float4x4 _BoneMatrices[8]; // 假设有最多8个骨骼

            v2f vert (appdata v) {
                v2f o;
                o.uv = v.uv;

                // 计算加权后的顶点位置
                float4 finalPos = float4(0, 0, 0, 1);
                for (int i = 0; i < 4; ++i) {
                    int index = v.boneIndices[i];
                    if (index < 8) {
                        finalPos += mul(_BoneMatrices[index], float4(v.vertex.xyz, 1.0)) * v.boneWeights[i];
                    }
                }

                o.vertex = UnityObjectToClipPos(finalPos);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                fixed4 col = tex2D(_MainTex, i.uv);
                return col;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

### 总结

以上示例展示了如何在 Unity Shader 中使用各个顶点属性来实现不同的功能。每个属性都有其独特的用途，选择合适的属性组合可以实现从简单的几何渲染到复杂的材质模拟的各种效果。希望这些示例能够帮助你更好地理解顶点信息在 Shader 编程中的作用。