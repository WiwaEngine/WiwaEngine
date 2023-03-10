namespace Wiwa
{
    using EntityId = System.UInt64;
    public class PhysicsManager
    {
        public static void SetLinearVelocity(EntityId id, Vector3 velocity)
        {
            InternalCalls.SetLinearVelocity(id, velocity);
        }

        public static bool AddBodyToLog(EntityId id)
        {
            return InternalCalls.AddBodyToLog(id);
        }

        public static bool RemoveBodyFromLog(EntityId id)
        {
            return InternalCalls.RemoveBodyFromLog(id);
        }

    }
}

