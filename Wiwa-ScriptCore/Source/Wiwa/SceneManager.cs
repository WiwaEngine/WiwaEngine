

namespace Wiwa.SceneManagment
{
    public static class SceneManager 
    {
        public static void LoadScene(string path)
        {
            InternalCalls.LoadSceneByPathIntr(path);
        }
        public static void LoadScene(int index)
        {
            InternalCalls.LoadSceneByIndexIntr(index);
        }
    }
}