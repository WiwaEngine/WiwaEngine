﻿using System.Runtime.CompilerServices;

namespace Wiwa
{
    using EntityId = System.UInt64;
    internal class InternalCalls
    {
        #region Log
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string log, int param);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param, out Vector3 result);
        #endregion
        #region ECS
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T GetComponent<T>(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T AddComponent<T>(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ApplySystem(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId CreateEntity();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DestroyEntity(EntityId eid);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AddMeshToEntity(EntityId eid, string model, string mat);
        #endregion
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyDownIntr(KeyCode key);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsMouseButtonPressedIntr(int button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseXIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseYIntr();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseXDeltaIntr();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseYDeltaIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsButtonPressedIntr(Gamepad gamepad, KeyCode button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetAxisIntr(Gamepad gamepad, GamepadAxis axis);
        #endregion
        #region Time
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTimeIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetTimeIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetFPSIntr(int fps);
        #endregion
        #region Camera
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static System.UInt64 GetActiveCamera();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCameraPosition(System.UInt64 camid, Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraLookAt(System.UInt64 camid, Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCameraFront(System.UInt64 camid, Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCameraRotation(System.UInt64 camid, Vector3 vec);
        #endregion
        #region Resources
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong LoadResourceModel(string str);
        #endregion
        //#region Audio
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void PlayMusic(string str);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void StopMusic(string str);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void PlaySound(string str, EntityId entity);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void StopSound(string str, EntityId entity);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void StopAllEvents();
        //#endregion
    }
}
