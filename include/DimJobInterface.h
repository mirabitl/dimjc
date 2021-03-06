#ifndef _DimJobInterface_h
#define _DimJobInterface_h

// -- std headers
#include <vector>
#include <string>
#include <stdint.h>

// -- dim headers
#include "dic.hxx"

// -- json headers
#include "json/json.h"

/**
 *  @brief  DimJobInterface class
 */
class DimJobInterface : public DimClient, private DimTimer
{
public:
	/**
	 *  @brief  Constructor
	 */
	DimJobInterface();

	/**
	 *  @brief  Destructor
	 */
	~DimJobInterface();

	/**
	 *  @brief  Load a json file
	 *
	 *  @param  fileName the json file name to load
	 */
	void loadJSON(const std::string &fileName);

	/**
	 *  @brief  List the whole content in standard output
	 */
	void list();

	/**
	 *  @brief  Start all jobs of the given host
	 *
	 *  @param  hostName the host name to start all jobs on
	 */
	void startJobs(const std::string &hostName);

	/**
	 *  @brief  Start a specific job on a host
	 *
	 *  @param  hostName the host name on which to start the job on
	 *  @param  jobName the job name to start
	 */
	void startJob(const std::string &hostName, const std::string &jobName);

	/**
	 *  @brief  Clear all jobs of a given host
	 *
	 *  @param  hostName the host name to clear jobs
	 */
	void clearHostJobs(const std::string &hostName);

	/**
	 *  @brief  Clear all jobs of hosts
	 */
	void clearAllJobs();

	/**
	 *  @brief  Query job status to job control
	 */
	void status();

	/**
	 *  @brief  Kill a job (by pid) on a given host by sending a specific signal
	 *
	 *  @param  hostName the target host name
	 *  @param  pid the process id to kill
	 *  @param  sig the signal to send
	 */
	void killJob(const std::string &hostName, uint32_t pid, uint32_t sig = 9);

	/**
	 *  @brief  Kill a job (by pid) on a given host by sending a specific signal
	 *
	 *  @param  hostName the target host name
	 *  @param  jobName the target job name
	 *  @param  pid the process id to kill
	 *  @param  sig the signal to send
	 */
	void restartJob(const std::string &hostName, const std::string &jobName,
			uint32_t pid, uint32_t sig = 9);

	/**
	 *  @brief  Get the process status list as a json string
	 *
	 *  @return  The serialized process status list json string
	 */
	std::string processStatusList() const;

	/**
	 *  @brief  Get the process status of a given host
	 *
	 *  @param  hostName the target host name
	 *
	 *  @return  the json value of the host process status
	 */
	Json::Value processStatus(const std::string &hostName) const;

	/**
	 *  @brief  Get the process list as a json string
	 *
	 *  @return  The serialized process list json string
	 */
	std::string processJobList() const;

	/**
	 *  @brief  Get the process status json value
	 */
	const Json::Value &getProcessStatusValue() const;

	/**
	 *  @brief  Get the root value containing the whole interface information
	 *
	 *  @return  The root json value
	 */
	const Json::Value &getRoot() const;

	/**
	 *  @brief  Clear the interface
	 */
	void clear();

	/**
	 *  @brief  Start an automatic timer to query jobs status
	 *
	 *  @param  nSeconds the query time interval
	 */
	void startTimer(int nSeconds);

	/**
	 * @brief  Stop the automatic jobs status query timer
	 */
	void stopTimer();

protected:
	/**
	 *  @brief  Dim info handler to receive updates from the server
	 */
	virtual void infoHandler();

	/**
	 *  @brief  Call back method called when job status of a host are received.
	 *          To be re-implemented by users
	 *          WARNING : This method is called from different thread than the main one
	 *
	 *  @param  The target host name
	 */
	virtual void statusReceived(const std::string &/*hostName*/) {}

private:
	/**
	 *  @brief  Dim timer handler
	 */
	void timerHandler();

private:

	Json::Value                 m_root;              ///< The root json value
	std::vector<std::string>    m_djcNames;          ///< The host name list
	std::vector<Json::Value>    m_processList;       ///< The process json value list
	std::vector<Json::Value>    m_jobValue;          ///< The job json value list
	Json::Value                 m_processArray;      ///< The processes json value

	char                        m_jobbuffer[8192];   ///< The internal buffer to store received dim info buffer
	std::vector<DimInfo*>       m_jobInfo;           ///< The list of dim info

	mutable pthread_mutex_t     m_mutex;             ///< The pthread mutex to avoid data rac conditions
	int                         m_timerPeriod;       ///< The timer period (unit seconds)
};


#endif

