### 1. Lighting Experiments

---

#### 🧱 Diffuse / Specular / Emission Maps

<div align="center">

| Diffuse Map | Specular Map |
|---|---|
| <img src="https://github.com/user-attachments/assets/0e9f0435-6c89-433b-8e50-55577fd95687" width="300"/> | <img src="https://github.com/user-attachments/assets/e56ed70f-5399-4ed1-a9f2-251b17a6cb0e" width="300"/> |

| Emission Map | Diffuse (alternate view) |
|---|---|
| <img src="https://github.com/user-attachments/assets/0f5ecad7-6e33-4d12-b7ed-b8ec4b73d97d" width="300"/> | <img src="https://github.com/user-attachments/assets/022fa701-1ecf-4a14-a8ac-e6e4d3bc29db" width="300"/> |

</div>

---

#### 💡 Lighting Models

<div align="center">

| Directional Light | Point Light with Attenuation |
|---|---|
| <img src="https://github.com/user-attachments/assets/fad87582-b48b-4163-aa6b-0eb7fe2e8673" width="450"/> | <img src="https://github.com/user-attachments/assets/0bb935e7-7c87-4d9d-9217-23fbfd0b0be1" width="450"/> |

</div>

#### 🔦 Flashlight Projection with Cookie Texture

A projected spotlight (“cookie texture”) was implemented by projecting a texture through the spotlight frustum using light-space coordinates. The flashlight projection supports attenuation, smooth spotlight falloff, and texture-based masking effects.

<div align="center">

<img src="https://github.com/user-attachments/assets/84323c05-cec3-4826-944c-ef87486c0b0d" width="400"/>

</div>

#### 💡 Added Multiple Lights and imgui support
<div align="center">
<img width="500" alt="Screenshot 2026-05-15 at 18 34 34" src="https://github.com/user-attachments/assets/29ec1b4e-6376-406d-8893-b96ec9684ef3" />
</div>
