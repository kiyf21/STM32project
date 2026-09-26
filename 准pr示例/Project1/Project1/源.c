#include <math.h>

// 准PR控制器结构体
typedef struct {
    // 参数
    float Kp;       // 比例系数
    float Kr;       // 谐振系数
    float w0;       // 谐振角频率 (rad/s)
    float wc;       // 截止带宽 (rad/s)
    float Ts;       // 采样周期 (s)

    // 离散化系数
    float b0_prime;
    float b2_prime;
    float a1_prime;
    float a2_prime;

    // 状态变量
    float e_prev1;  // e[k-1]
    float e_prev2;  // e[k-2]
    float yr_prev1; // yr[k-1]
    float yr_prev2; // yr[k-2]
} QPRController;

// 初始化控制器
void QPR_Init(QPRController* ctrl,
    float Kp, float Kr,
    float w0, float wc,
    float Ts)
{
    ctrl->Kp = Kp;
    ctrl->Kr = Kr;
    ctrl->w0 = w0;
    ctrl->wc = wc;
    ctrl->Ts = Ts;

    // 计算中间变量
    float a = 2.0f / Ts;
    float a_sq = a * a;
    float w0_sq = w0 * w0;
    float b0 = 2.0f * Kr * wc * a;

    // 计算分母系数
    float a0 = a_sq + 2.0f * wc * a + w0_sq;
    float a1 = -2.0f * a_sq + 2.0f * w0_sq;
    float a2 = a_sq - 2.0f * wc * a + w0_sq;

    // 归一化系数
    ctrl->b0_prime = b0 / a0;
    ctrl->b2_prime = -b0 / a0;  // b2 = -b0
    ctrl->a1_prime = a1 / a0;
    ctrl->a2_prime = a2 / a0;

    // 初始化状态
    ctrl->e_prev1 = 0.0f;
    ctrl->e_prev2 = 0.0f;
    ctrl->yr_prev1 = 0.0f;
    ctrl->yr_prev2 = 0.0f;
}

// 执行控制计算
float QPR_Update(QPRController* ctrl, float ref, float fdb) {
    // 计算当前误差
    float e = ref - fdb;

    // 计算谐振部分输出
    float yr = ctrl->b0_prime * e
        + ctrl->b2_prime * ctrl->e_prev2
        - ctrl->a1_prime * ctrl->yr_prev1
        - ctrl->a2_prime * ctrl->yr_prev2;

    // 比例部分 + 谐振部分
    float u = ctrl->Kp * e + yr;

    // 更新状态变量
    ctrl->e_prev2 = ctrl->e_prev1;
    ctrl->e_prev1 = e;
    ctrl->yr_prev2 = ctrl->yr_prev1;
    ctrl->yr_prev1 = yr;

    return u;
}
int main() {
    QPRController pr_ctrl;

    // 初始化参数 (示例：50Hz系统)
    float Kp = 0.5f;       // 比例系数
    float Kr = 10.0f;      // 谐振系数
    float f0 = 50.0f;      // 谐振频率 (Hz)
    float w0 = 2 * M_PI * f0;  // 角频率
    float wc = 5.0f;       // 截止带宽 (rad/s)
    float Ts = 0.001f;     // 1kHz采样

    QPR_Init(&pr_ctrl, Kp, Kr, w0, wc, Ts);

    // 实时控制循环
    while (1) {
        float ref = ...;    // 获取参考值
        float fdb = ...;    // 获取反馈值
        float u = QPR_Update(&pr_ctrl, ref, fdb);

        // 应用控制量u到被控对象
        ...

            delay(Ts);  // 等待下一个采样周期
    }
}