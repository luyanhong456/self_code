<?php
/**
 * Auto generated from jtmf_peomanage.proto at 2016-03-16 12:06:43
 *
 * ad.peomanage package
 */

namespace Ad\Peomanage {
/**
 * CQueryTaskItem message
 */
class CQueryTaskItem extends \ProtobufMessage
{
    /* Field index constants */
    const ID = 1;
    const NAME = 2;
    const TASKDESC = 3;
    const USERCOUNTLEVEL = 4;
    const STATUS = 5;
    const EXTUSERCOUNT = 6;
    const DELFLAG = 7;
    const INSERTTIME = 8;
    const LASTUPDTIME = 9;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::ID => array(
            'name' => 'id',
            'required' => false,
            'type' => 5,
        ),
        self::NAME => array(
            'name' => 'name',
            'required' => false,
            'type' => 7,
        ),
        self::TASKDESC => array(
            'name' => 'taskdesc',
            'required' => false,
            'type' => 7,
        ),
        self::USERCOUNTLEVEL => array(
            'name' => 'usercountlevel',
            'required' => false,
            'type' => 5,
        ),
        self::STATUS => array(
            'name' => 'status',
            'required' => false,
            'type' => 5,
        ),
        self::EXTUSERCOUNT => array(
            'name' => 'extusercount',
            'required' => false,
            'type' => 5,
        ),
        self::DELFLAG => array(
            'name' => 'delflag',
            'required' => false,
            'type' => 5,
        ),
        self::INSERTTIME => array(
            'name' => 'inserttime',
            'required' => false,
            'type' => 5,
        ),
        self::LASTUPDTIME => array(
            'name' => 'lastupdtime',
            'required' => false,
            'type' => 5,
        ),
    );

    /**
     * Constructs new message container and clears its internal state
     *
     * @return null
     */
    public function __construct()
    {
        $this->reset();
    }

    /**
     * Clears message values and sets default ones
     *
     * @return null
     */
    public function reset()
    {
        $this->values[self::ID] = null;
        $this->values[self::NAME] = null;
        $this->values[self::TASKDESC] = null;
        $this->values[self::USERCOUNTLEVEL] = null;
        $this->values[self::STATUS] = null;
        $this->values[self::EXTUSERCOUNT] = null;
        $this->values[self::DELFLAG] = null;
        $this->values[self::INSERTTIME] = null;
        $this->values[self::LASTUPDTIME] = null;
    }

    /**
     * Returns field descriptors
     *
     * @return array
     */
    public function fields()
    {
        return self::$fields;
    }

    /**
     * Sets value of 'id' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setId($value)
    {
        return $this->set(self::ID, $value);
    }

    /**
     * Returns value of 'id' property
     *
     * @return int
     */
    public function getId()
    {
        return $this->get(self::ID);
    }

    /**
     * Sets value of 'name' property
     *
     * @param string $value Property value
     *
     * @return null
     */
    public function setName($value)
    {
        return $this->set(self::NAME, $value);
    }

    /**
     * Returns value of 'name' property
     *
     * @return string
     */
    public function getName()
    {
        return $this->get(self::NAME);
    }

    /**
     * Sets value of 'taskdesc' property
     *
     * @param string $value Property value
     *
     * @return null
     */
    public function setTaskdesc($value)
    {
        return $this->set(self::TASKDESC, $value);
    }

    /**
     * Returns value of 'taskdesc' property
     *
     * @return string
     */
    public function getTaskdesc()
    {
        return $this->get(self::TASKDESC);
    }

    /**
     * Sets value of 'usercountlevel' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setUsercountlevel($value)
    {
        return $this->set(self::USERCOUNTLEVEL, $value);
    }

    /**
     * Returns value of 'usercountlevel' property
     *
     * @return int
     */
    public function getUsercountlevel()
    {
        return $this->get(self::USERCOUNTLEVEL);
    }

    /**
     * Sets value of 'status' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setStatus($value)
    {
        return $this->set(self::STATUS, $value);
    }

    /**
     * Returns value of 'status' property
     *
     * @return int
     */
    public function getStatus()
    {
        return $this->get(self::STATUS);
    }

    /**
     * Sets value of 'extusercount' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setExtusercount($value)
    {
        return $this->set(self::EXTUSERCOUNT, $value);
    }

    /**
     * Returns value of 'extusercount' property
     *
     * @return int
     */
    public function getExtusercount()
    {
        return $this->get(self::EXTUSERCOUNT);
    }

    /**
     * Sets value of 'delflag' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setDelflag($value)
    {
        return $this->set(self::DELFLAG, $value);
    }

    /**
     * Returns value of 'delflag' property
     *
     * @return int
     */
    public function getDelflag()
    {
        return $this->get(self::DELFLAG);
    }

    /**
     * Sets value of 'inserttime' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setInserttime($value)
    {
        return $this->set(self::INSERTTIME, $value);
    }

    /**
     * Returns value of 'inserttime' property
     *
     * @return int
     */
    public function getInserttime()
    {
        return $this->get(self::INSERTTIME);
    }

    /**
     * Sets value of 'lastupdtime' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setLastupdtime($value)
    {
        return $this->set(self::LASTUPDTIME, $value);
    }

    /**
     * Returns value of 'lastupdtime' property
     *
     * @return int
     */
    public function getLastupdtime()
    {
        return $this->get(self::LASTUPDTIME);
    }
}
}