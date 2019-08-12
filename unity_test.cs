using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.Runtime.InteropServices;

public class move : MonoBehaviour
{
    [DllImport("openvino_test.dll")]
    public extern static void initHumanPose3D();

    [DllImport("openvino_test.dll")]
    public extern static void nextFrame(float[] data);


    private float[] data;
    
    // Start is called before the first frame update
    void Start()
    {
        initHumanPose3D();
        data = new float[10 * 18 * 3 + 1];
    }

    // Update is called once per frame
    void Update()
    {
        nextFrame(data);
        int n = (int)(data[0]);
        Debug.Log("person:" + n);
    }
}
