#define THREADINLINE

#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <array.h>
#include <cpu.h>
#include <spl.h>
#include <spinlock.h>
#include <wchan.h>
#include <thread.h>
#include <threadlist.h>
#include <threadprivate.h>
#include <proc.h>
#include <current.h>
#include <synch.h>
#include <addrspace.h>
#include <mainbus.h>
#include <vnode.h>


/* Magic number used as a guard value on kernel thread stacks. */
#define THREAD_STACK_MAGIC 0xbaadf00d

/* Wait channel. A wchan is protected by an associated, passed-in spinlock. */
struct wchan {
	const char *wc_name;		/* name for this channel */
	struct threadlist wc_threads;	/* list of waiting threads */
};

/* Master array of CPUs. */
DECLARRAY(cpu, static __UNUSED inline);
DEFARRAY(cpu, static __UNUSED inline);
static struct cpuarray allcpus;

/* Used to wait for secondary CPUs to come online. */
static struct semaphore *cpu_startup_sem;

/*
* High level, machine-independent context switch code.
*
* The current thread is queued appropriately and its state is changed
* to NEWSTATE; another thread to run is selected and switched to.
*
* If NEWSTATE is S_SLEEP, the thread is queued on the wait channel
* WC, protected by the spinlock LK. Otherwise WC and Lk should be
* NULL.
*/
static
void
thread_switch(threadstate_t newstate, struct wchan *wc, struct spinlock *lk)
{
	struct thread *cur, *next; // ���� ��ġ �����Ϳ� ���� ��ġ ������ ����
	int spl;

	DEBUGASSERT(curcpu->c_curthread == curthread); // lib.h�� �ִ� DEBUGASSERT�� �� ������ �� ��
	DEBUGASSERT(curthread->t_cpu == curcpu->c_self);

	/* Explicitly disable interrupts on this processor */
	spl = splhigh(); // spl�� 1 ����

	cur = curthread; // ���� �������� ���� ���� ��ġ �����Ϳ� ����

					 /*
					 * If we're idle, return without doing anything. This happens
					 * when the timer interrupt interrupts the idle loop.
					 */
	if (curcpu->c_isidle) {
		splx(spl);
		return;
	}

	/* Check the stack guard band. */
	thread_checkstack(cur);

	/* Lock the run queue. */
	spinlock_acquire(&curcpu->c_runqueue_lock);

	/* Micro-optimization: if nothing to do, just return */
	if (newstate == S_READY && threadlist_isempty(&curcpu->c_runqueue)) {
		spinlock_release(&curcpu->c_runqueue_lock);
		splx(spl);
		return;
	}

	/* Put the thread in the right place. */
	switch (newstate) {
	case S_RUN: // �������� ���
		panic("Illegal S_RUN in thread_switch\n");
	case S_READY: // Ready ����
		thread_make_runnable(cur, true /*have lock*/);
		break;
	case S_SLEEP: // wait ����
		cur->t_wchan_name = wc->wc_name;
		/*
		* Add the thread to the list in the wait channel, and
		* unlock same. To avoid a race with someone else
		* calling wchan_wake*, we must keep the wchan's
		* associated spinlock locked from the point the
		* caller of wchan_sleep locked it until the thread is
		* on the list.
		*/
		threadlist_addtail(&wc->wc_threads, cur);
		spinlock_release(lk);
		break;
	case S_ZOMBIE: // ���� ���
		cur->t_wchan_name = "ZOMBIE";
		threadlist_addtail(&curcpu->c_zombies, cur);
		break;
	}
	cur->t_state = newstate; // newstate�� ���� ���� ���� �������� ���°����� ����

							 /*
							 * Get the next thread. While there isn't one, call cpu_idle().
							 * curcpu->c_isidle must be true when cpu_idle is
							 * called. Unlock the runqueue while idling too, to make sure
							 * things can be added to it.
							 *
							 * Note that we don't need to unlock the runqueue atomically
							 * with idling; becoming unidle requires receiving an
							 * interrupt (either a hardware interrupt or an interprocessor
							 * interrupt from another cpu posting a wakeup) and idling
							 * *is* atomic with respect to re-enabling interrupts.
							 *
							 * Note that c_isidle becomes true briefly even if we don't go
							 * idle. However, because one is supposed to hold the runqueue
							 * lock to look at it, this should not be visible or matter.
							 */

							 /* The current cpu is now idle. */
	curcpu->c_isidle = true; // ���� cpu�� idle ���·� ����
	do { // ù ��° next�� null�� �ƴ� ��� ������ do ~ while ����
		next = threadlist_remhead(&curcpu->c_runqueue);
		if (next == NULL) {
			spinlock_release(&curcpu->c_runqueue_lock); // ����ť unlock
			cpu_idle(); // cpu idle ���·� ����
			spinlock_acquire(&curcpu->c_runqueue_lock);
		}
	} while (next == NULL); // next�� Null�� �ƴҶ����� �ݺ� ����
	curcpu->c_isidle = false; // ���� cpu�� Non-idle ���·� ����

							  /*
							  * Note that curcpu->c_curthread may be the same variable as
							  * curthread and it may not be, depending on how curthread and
							  * curcpu are defined by the MD code. We'll assign both and
							  * assume the compiler will optimize one away if they're the
							  * same.
							  */
	curcpu->c_curthread = next; // �������� ���� ���� ��ġ ������ ������ ����
	curthread = next; // ���� ������ ���� ���� ��ġ ������ ������ ����

					  /* do the switch (in assembler in switch.S) */
	switchframe_switch(&cur->t_context, &next->t_context);

	/*
	* When we get to this point we are either running in the next
	* thread, or have come back to the same thread again,
	* depending on how you look at it. That is,
	* switchframe_switch returns immediately in another thread
	* context, which in general will be executing here with a
	* different stack and different values in the local
	* variables. (Although new threads go to thread_startup
	* instead.) But, later on when the processor, or some
	* processor, comes back to the previous thread, it's also
	* executing here with the *same* value in the local
	* variables.
	*
	* The upshot, however, is as follows:
	*
	*    - The thread now currently running is "cur", not "next",
	*      because when we return from switchrame_switch on the
	*      same stack, we're back to the thread that
	*      switchframe_switch call switched away from, which is
	*      "cur".
	*
	*    - "cur" is _not_ the thread that just *called*
	*      switchframe_switch.
	*
	*    - If newstate is S_ZOMB we never get back here in that
	*      context at all.
	*
	*    - If the thread just chosen to run ("next") was a new
	*      thread, we don't get to this code again until
	*      *another* context switch happens, because when new
	*      threads return from switchframe_switch they teleport
	*      to thread_startup.
	*
	*    - At this point the thread whose stack we're now on may
	*      have been migrated to another cpu since it last ran.
	*
	* The above is inherently confusing and will probably take a
	* while to get used to.
	*
	* However, the important part is that code placed here, after
	* the call to switchframe_switch, does not necessarily run on
	* every context switch. Thus any such code must be either
	* skippable on some switches or also called from
	* thread_startup.
	*/


	/* Clear the wait channel and set the thread state. */
	cur->t_wchan_name = NULL; // ���ä�� �ʱ�ȭ
	cur->t_state = S_RUN; // ������ ���� ������·� ����

						  /* Unlock the run queue. */
	spinlock_release(&curcpu->c_runqueue_lock);

	/* Activate our address space in the MMU. */
	as_activate();

	/* Clean up dead threads. */
	exorcise();

	/* Turn interrupts back on. */
	splx(spl);
}

/*Kern / thread / thread.c���� context switch ������ �߰��� �� �־����ϴ�.
���� ��ġ���� stack�� üũ�ϰ� ���� ���ο� ���¿� ���� �б⹮�� �����մϴ�.
�׸��� �� ���� ���� ���� ��ġ�� �����ϰ� cpu�� ���¸� idle ���·� �����Ͽ� ���� ������ġ�� ���� Null�� �ƴҶ����� ���� ť�� unlock, cpu�� idle���·� �����ϸ� �ݺ� �����մϴ�.
���� ������ġ�� ���� Null�� �ƴ� ��� ���� ������ġ ���� ���� ������ ������ �������ְ� frame_switch �Լ��� ���Ͽ� context switch�մϴ�. 
�� �� ä���� �ʱ�ȭ�ϰ� ���� ���¸� ������·� �����մϴ�.���� ���� ť�� �����ϰ� MMU�� ���Ͽ� �ּҰ����� Ȱ��ȭ�� ��, ���� �����带 �����ϰ� ���ͷ�Ʈ �߻� �� �ܰ�� �ǵ��ư��ϴ�.
*/