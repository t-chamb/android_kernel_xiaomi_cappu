/* See pvr_guilty_kill.h. */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/cred.h>
#include <linux/uidgid.h>

#include "pvr_guilty_kill.h"

#define AID_APP_START 10000

void PVRKillGuiltyApp(IMG_PID pid, IMG_UINT32 reason)
{
	struct pid *p;
	struct task_struct *task;
	uid_t uid;
	char comm[TASK_COMM_LEN];

	rcu_read_lock();
	p = find_vpid(pid);
	task = p ? pid_task(p, PIDTYPE_PID) : NULL;
	if (!task) {
		rcu_read_unlock();
		return;
	}
	uid = from_kuid(&init_user_ns, task_uid(task));
	get_task_comm(comm, task);
	if (uid % 100000 >= AID_APP_START)
		kill_pid(p, SIGKILL, 1);
	rcu_read_unlock();

	if (uid % 100000 < AID_APP_START)
		pr_err("PVR: context of %d (%s, uid %u) caused a GPU lockup (reason %u); not an app, left running\n",
		       pid, comm, uid, reason);
	else
		pr_err("PVR: killed %d (%s, uid %u): its GPU context caused a lockup (reason %u)\n",
		       pid, comm, uid, reason);
}
